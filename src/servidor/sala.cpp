#include "servidor/sala.h"

#include <chrono>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "comun/eventos_servidor.h"
#include "comun/i_modelo.h"
#include "comun/lock.h"
#include "comun/log.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"

// Descomentar esta línea para activar los mensajes de depuración de tiempos
//#define DEPURAR_TIEMPOS 1

/**
 * Cada cuánto se procesan eventos
 */
#define TICK_MS 20

// Dejar un 20% del tiempo de actualización como margen.
#define DELAY_EV ((TICK_MS * 20) / 100)

namespace servidor {

Sala::Sala(const std::string& nombre_, size_t capacidad_maxima, IModelo* juego) 
: modelo(juego), nombre(nombre_), capacidad(capacidad_maxima)
{ }

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
    sala_abierta = otro.sala_abierta;
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

    if (jugadores.size() == 0)
        terminar = true;
}

void Sala::configurar_recepcion_eventos() {
    for (auto it=clientes.begin();it!=clientes.end();++it) {
        Cliente& cliente = *it->first;
        IJugador *jugador = &it->second;
        modelo->crear_jugador(&it->second);
        
        cliente.al_recibir_datos([this, jugador] (const nlohmann::json& data) {
            cola_eventos.push({jugador, data});
        });
    }
}

bool Sala::set_casa_cliente(Cliente& cliente, const std::string& casa) {
    clientes.at(&cliente).set_casa(casa);
    return true;
}

void Sala::listar_jugadores(Cliente& cliente) {
    std::vector<nlohmann::json> data_clientes;

    for (auto it=clientes.begin();it!=clientes.end();++it) {
        ConexionJugador& jugador = it->second;
        data_clientes.push_back({
            {"nombre", jugador.obtener_nombre()},
            {"casa", jugador.obtener_casa()},
            {"listo", jugador.esta_listo()}
        });
    }

    cliente.enviar({
        {"estado", "OK"},
        {"jugadores", data_clientes}
    });
}

void Sala::iniciar_partida(Cliente& cliente) {
    if (partida_iniciada)
        throw std::runtime_error("Esta partida ya fue iniciada");
    
    sala_abierta = false;

    ConexionJugador& jugador_actual = clientes.at(&cliente);
    jugador_actual.set_estado(true);

    bool todos_listos = true;

    for (auto it=clientes.begin();it!=clientes.end();++it) {
        ConexionJugador& jugador = it->second;
        todos_listos &= jugador.esta_listo();
    }

    if (todos_listos) {
        hilo_partida = std::thread(&Sala::jugar, this);
    }
}

static int64_t medir_tiempo_ms(std::function<void(void)> funcion) {
    using namespace std::chrono;
    auto t1 = steady_clock::now();
    funcion();
    auto t2 = steady_clock::now();
    return duration_cast<milliseconds>(t2 - t1).count();
}

void Sala::jugar() {
    using namespace std::chrono;
    try {
        if (partida_iniciada)
            throw std::runtime_error("La partida ya fue iniciada");
        
        partida_iniciada = true;
        terminar = false;
        configurar_recepcion_eventos();
        
        modelo->iniciar_partida();

        while (!terminar && !modelo->esperar_sincronizacion_inicial()) {
            std::pair<IJugador*, nlohmann::json> evento = cola_eventos.pull();
            actualizar_modelo(evento.first, evento.second);
        }

        auto base = steady_clock::now();
        while (!terminar && !modelo->partida_terminada()) {
            auto timeout = base + milliseconds(TICK_MS);
            auto ms_eventos = medir_tiempo_ms([&] () {
                auto timeout_eventos = base + milliseconds(TICK_MS - DELAY_EV);
                while (steady_clock::now() < timeout_eventos) {
                    std::pair<IJugador*, nlohmann::json> evento;
                    // Esto no es RAII por una buena razón que no entra en un
                    //  comentario de una línea (ni de dos).
                    if (cola_eventos.pull(timeout_eventos, evento))
                        actualizar_modelo(evento.first, evento.second);
                }
            });

            if (ms_eventos > TICK_MS) {
                log_advertencia("El procesamiento de eventos tardó %lu ms"
                    " (TICK_MS=%lu ms)", ms_eventos, TICK_MS);
            }

            auto ms_actualizacion = medir_tiempo_ms([&] () {
                std::this_thread::sleep_until(timeout);
                for (int i=0;i<TICK_MS/TICK_MS;i++)
                    modelo->actualizar(TICK_MS);
            });                
            
            if (ms_actualizacion > TICK_MS) {
                log_advertencia("El modelo tardó %lu ms en actualizarse"
                    " (TICK_MS=%lu ms)", ms_eventos, TICK_MS);
            }

            base = timeout;
        }
    } catch(const std::exception& e) {
        log_error("Explotó la partida en la sala %s\n --> %s", nombre, 
            e.what());
    } catch(...) {
        log_error("Explotó la partida en la sala %s con un error desconocido.");
    }
}

bool Sala::puede_unirse() const {
    return sala_abierta &&
        (jugadores.size() < capacidad) && !partida_iniciada;
}

void Sala::terminar_partida() {
    terminar = true;
    sala_abierta = false;

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

void Sala::actualizar_modelo(IJugador* jugador, const nlohmann::json& evento) {    
    Lock l(lock_modelo);
    
    evento_servidor_t id = evento.at("id").get<evento_servidor_t>();

    try {
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
                    modelo->atacar(
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
            
            case EVS_JUGADOR_LISTO:
                    modelo->jugador_listo(jugador);
                break;

            default:
                throw std::runtime_error("actualizar_modelo: Evento desconocido");
        }
    } catch(const std::exception& e) {
        log_advertencia("Explosión controlada: %s\n", e.what());
    } // Dejar que otras excepciones asciendan
}

} // namespace servidor
