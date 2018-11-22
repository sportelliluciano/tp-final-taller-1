#include "servidor/sala.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "conexion/eventos_servidor.h"
#include "conexion/lock.h"
#include "conexion/i_modelo.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"
#include "servidor/mock_modelo.h"
#include "modelo/juego.h"

namespace servidor {

Sala::Sala(const std::string& nombre_, size_t capacidad_maxima) 
: modelo(new modelo::Juego()), nombre(nombre_)
{
    capacidad = capacidad_maxima;
    std::ifstream f_mapa("../data/mapa2.json");
    f_mapa >> mapa;
    
    std::ifstream f_edificios("../data/edificios.json");
    f_edificios >> edificios;

    std::ifstream f_ejercito("../data/ejercito.json");
    f_ejercito >> ejercito;

    modelo->inicializar(mapa, edificios, ejercito);
}

Sala::Sala(Sala&& otro) {
    if (partida_iniciada || otro.partida_iniciada) {
        throw std::runtime_error(
            "No se puede mover la sala con la partida iniciada");
    }

    modelo = std::move(otro.modelo);
    jugadores = std::move(otro.jugadores);
    clientes = std::move(otro.clientes);
    ultimo_id = otro.ultimo_id;
    terminar = otro.terminar;
    capacidad = otro.capacidad;
    partida_iniciada = otro.partida_iniciada;
    mapa = std::move(otro.mapa);
    edificios = std::move(otro.edificios);
    ejercito = std::move(otro.ejercito);
}

void Sala::agregar_cliente(Cliente& cliente) {
    if (!puede_unirse())
        throw std::runtime_error("No se puede unir a esta sala");

    ultimo_id++;
    
    clientes.emplace(&cliente, ConexionJugador(cliente, ultimo_id));
    ConexionJugador& nuevo_jugador = clientes.at(&cliente);
    
    jugadores.insert({ultimo_id, &nuevo_jugador});
}

void Sala::eliminar_cliente(Cliente& cliente) {
    if (partida_iniciada)
        throw std::runtime_error("La partida ya fue iniciada");
    
    jugadores.erase(clientes.at(&cliente).obtener_id());
    clientes.erase(&cliente);
}

void Sala::notificar_desconexion(Cliente& cliente) {
    if (partida_iniciada) {
        modelo->jugador_desconectado(&clientes.at(&cliente));
    } 
    jugadores.erase(clientes.at(&cliente).obtener_id());
    clientes.erase(&cliente);
}

void Sala::configurar_recepcion_eventos() {
    for (auto it=clientes.begin();it!=clientes.end();++it) {
        Cliente& cliente = *it->first;
        IJugador *jugador = &it->second;
        modelo->crear_jugador(&it->second);
        
        cliente.al_recibir_datos([this, jugador] (const nlohmann::json& data) {
            actualizar_modelo(jugador, data);
        });
    }
}

void Sala::iniciar_partida(Cliente& cliente) {
    if (partida_iniciada)
        throw std::runtime_error("Esta partida ya fue iniciada");
    
    cliente.enviar(mapa);
    cliente.enviar(edificios);
    cliente.enviar(ejercito);

    clientes.at(&cliente).set_estado(true);

    bool todos_listos = true;

    for (auto it=clientes.begin();it!=clientes.end();++it) {
        ConexionJugador& jugador = it->second;

        todos_listos &= jugador.esta_listo();
    }

    if (todos_listos) {
        for (auto it=clientes.begin();it!=clientes.end();++it) {
            Cliente* jugador = it->first;
            jugador->enviar({
                {"tipo", "juego_iniciando"},
                {"id_jugador", it->second.obtener_id()}
            });
        }
        hilo_partida = std::thread(&Sala::jugar, this);
    }
}

void Sala::jugar() {
    try {
        if (partida_iniciada)
            throw std::runtime_error("La partida ya fue iniciada");
        
        partida_iniciada = true;
        terminar = false;
        configurar_recepcion_eventos();
        
        modelo->iniciar_partida();
        
        while (!terminar && !modelo->partida_terminada()) {
            lock_modelo.lock();
            modelo->actualizar(TICK_MS);
            lock_modelo.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
        }
    } catch(const std::exception& e) {
        std::cerr << "Explotó la partida en la sala " << nombre << std::endl;
        std::cerr << "--> " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Explotó la partida en la sala " << nombre 
                  << " con un error desconocido." << std::endl;
    }
}

bool Sala::puede_unirse() const {
    return (jugadores.size() >= capacidad) || !partida_iniciada;
}

void Sala::terminar_partida() {
    terminar = true;

    if (hilo_partida.joinable())
        hilo_partida.join();
}

size_t Sala::obtener_capacidad() {
    return capacidad;
}

int Sala::cantidad_jugadores_conectados() {
    return clientes.size();
}

Sala::~Sala() { }

// TODO: Chequear excepciones // refactorizar esto
void Sala::actualizar_modelo(IJugador* jugador, const nlohmann::json& evento) {
    using namespace conexion;
    
    Lock l(lock_modelo);
    
    evento_servidor_t id = evento.at("id").get<evento_servidor_t>();

    switch(id) {
        case EVS_INICIAR_CONSTRUCCION:
            modelo->iniciar_construccion_edificio(
                jugador, 
                evento.at("clase")
            );
            break;
        
        case EVS_CANCELAR_CONSTRUCCION:
            modelo->cancelar_construccion_edificio(
                jugador, 
                evento.at("clase")
            );
            break;
        
        case EVS_UBICAR_EDIFICIO: 
            modelo->ubicar_edificio(
                jugador, 
                evento.at("celda").get<std::vector<int>>().at(0),
                evento.at("celda").get<std::vector<int>>().at(1),
                evento.at("clase")
            );
            break;
        
        case EVS_VENDER_EDIFICIO:
            modelo->vender_edificio(
                jugador,
                evento.at("id_edificio")
            );
            break;

        case EVS_INICIAR_ENTRENAMIENTO:
            modelo->iniciar_entrenamiento_tropa(
                jugador, 
                evento.at("clase")
            );
            break;
        
        case EVS_CANCELAR_ENTRENAMIENTO:
            modelo->cancelar_entrenamiento_tropa(
                jugador, 
                evento.at("clase")
            );
            break;
        
        case EVS_MOVER_TROPAS: 
            modelo->mover_tropas(
                jugador,
                evento.at("ids_tropa").get<std::unordered_set<int>>(),
                evento.at("posicion").get<std::vector<int>>().at(0),
                evento.at("posicion").get<std::vector<int>>().at(1)
            );
            break;

        case EVS_ATACAR_TROPA:
            modelo->atacar_tropa(
                jugador,
                evento.at("ids_tropa").get<std::unordered_set<int>>(),
                evento.at("id_atacado")
            );
            break;
        
        case EVS_COSECHADORA_INDICAR_ESPECIA:
            modelo->indicar_especia_cosechadora(
                jugador,
                evento.at("ids_tropa").get<std::unordered_set<int>>(),
                evento.at("celda").get<std::vector<int>>().at(0),
                evento.at("celda").get<std::vector<int>>().at(1)
            );
            break;
        
        default:
            throw std::runtime_error("actualizar_modelo: Evento desconocido");
    }
}

} // namespace servidor
