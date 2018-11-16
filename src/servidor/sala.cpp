#include "servidor/sala.h"

#include <chrono>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include "conexion/eventos_servidor.h"
#include "conexion/lock.h"
#include "conexion/i_modelo.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"
#include "servidor/mock_modelo.h"
//#include "modelo/juego.h"

namespace servidor {

Sala::Sala(size_t capacidad_maxima) 
: modelo(new MockModelo())
//: modelo(new modelo::Juego())
{
    capacidad = capacidad_maxima;
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
}

bool Sala::puede_unirse() const {
    return (jugadores.size() >= capacidad) || !modelo->partida_iniciada();
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
        terminar_partida();
    } else {
        eliminar_cliente(cliente);
    }
}

void Sala::configurar_recepcion_eventos() {
    for (auto it=clientes.begin();it!=clientes.end();++it) {
        Cliente& cliente = *it->first;
        IJugador *jugador = &it->second;
        modelo->crear_jugador(&it->second);
        
        cliente.al_recibir_datos(
            [this, jugador] (const nlohmann::json& data) {
                actualizar_modelo(jugador, data);
            }
        );
    }
}

void Sala::iniciar_partida(Cliente& cliente) {
    if (partida_iniciada)
        throw std::runtime_error("Esta partida ya fue iniciada");
    
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
                {"tipo", "juego_iniciando"}
            });
        }
        partida_iniciada = true;
        hilo_partida = std::thread(&Sala::jugar, this);
    }
}

void Sala::jugar() {
    if (modelo->partida_iniciada())
        throw std::runtime_error("La partida ya fue iniciada");
    
    configurar_recepcion_eventos();

    modelo->iniciar_partida();

    terminar = false;

    while (!terminar && !modelo->juego_terminado()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
        lock_modelo.lock();
        modelo->actualizar(TICK_MS);
        lock_modelo.unlock();
    }
}

void Sala::terminar_partida() {
    terminar = true;

    if (partida_iniciada)
        hilo_partida.join();
}

size_t Sala::obtener_capacidad() {
    return capacidad;
}

Sala::~Sala() { }

// TODO: Chequear excepciones
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
                evento.at("ids_tropa"),
                evento.at("posicion").get<std::vector<int>>().at(0),
                evento.at("posicion").get<std::vector<int>>().at(1)
            );
            break;

        case EVS_ATACAR_TROPA:
            modelo->atacar_tropa(
                jugador,
                evento.at("ids_tropa"),
                evento.at("id_atacado")
            );
            break;
        
        case EVS_COSECHADORA_INDICAR_ESPECIA:
            modelo->indicar_especia_cosechadora(
                jugador,
                evento.at("ids_tropa"),
                evento.at("celda").get<std::vector<int>>().at(0),
                evento.at("celda").get<std::vector<int>>().at(1)
            );
            break;
        
        default:
            throw std::runtime_error("actualizar_modelo: Evento desconocido");
    }
}

} // namespace servidor
