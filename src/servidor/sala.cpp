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
#include <utility>

#include "conexion/eventos_servidor.h"
#include "conexion/lock.h"
#include "conexion/i_modelo.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"
#include "servidor/mock_modelo.h"
#include "modelo/juego.h"

// Dejar un 20% del tiempo de actualización como márgen.
#define DELAY_EV ((TICK_MS * 20) / 100)

#define DEPURAR_TIEMPO_EVENTO(ev, expr) {\
    std::cout << "[" << timestamp() << "ms] "\
        << "t(" ev ") = " << \
     medir_tiempo_ms([&] () {\
        expr \
     }) << "ms" << std::endl;\
}

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

static int64_t medir_tiempo_ms(std::function<void(void)> funcion) {
    using namespace std::chrono;
    auto t1 = steady_clock::now();
    funcion();
    auto t2 = steady_clock::now();
    return duration_cast<milliseconds>(t2 - t1).count();
}

static int64_t timestamp() {
    using namespace std::chrono;
    static auto base = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - base).count();
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
        auto base = steady_clock::now();
#if DEPURAR_TIEMPOS
        auto un_seg = base + seconds(1);
        double promedio_eventos = 0;
        double promedio_actualizaciones = 0;
        double promedio_iteracion = 0;
        int cantidad_promedio = 0;
#endif
        while (!terminar && !modelo->partida_terminada()) {
#if DEPURAR_TIEMPOS
            promedio_iteracion += medir_tiempo_ms([&] () {
#endif
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
#if DEPURAR_TIEMPOS
                if (ms_eventos > TICK_MS) {
                    std::cout << "[" << timestamp() << "ms] "
                            << "[ADVERTENCIA]: El procesamiento de eventos tardó "
                            << ms_eventos << "ms (TICK_MS=" << TICK_MS << "ms)"
                            << std::endl;
                }
#endif
                auto ms_actualizacion = medir_tiempo_ms([&] () {
                    std::this_thread::sleep_until(timeout);
                    modelo->actualizar(TICK_MS);
                });
#if DEPURAR_TIEMPOS                
                if (ms_actualizacion > TICK_MS) {
                    std::cout << "[" << timestamp() << "ms] "
                            << "[ADVERTENCIA]: El modelo tardó " 
                            << ms_actualizacion << "ms en actualizarse"
                            << " (TICK_MS=" << TICK_MS << "ms)"
                            << std::endl;
                }
                promedio_actualizaciones += ms_actualizacion;
                promedio_eventos += ms_eventos;
                cantidad_promedio++;

                if (steady_clock::now() > un_seg) {
                    promedio_actualizaciones /= cantidad_promedio;
                    promedio_eventos /= cantidad_promedio;
                    promedio_iteracion /= cantidad_promedio;
                    std::cout << "[" << timestamp() << "ms] "
                            << "avg(actualizacion): " 
                            << promedio_actualizaciones << "ms" << std::endl;
                    std::cout << "[" << timestamp() << "ms] "
                            << "avg(eventos): " << promedio_eventos << "ms"
                            << std::endl;
                    std::cout << "[" << timestamp() << "ms] "
                            << "avg(iter): " << promedio_iteracion << "ms"
                            << std::endl;
                    promedio_iteracion = promedio_actualizaciones = promedio_eventos = 0;
                    cantidad_promedio = 0;
                    un_seg = steady_clock::now() + seconds(1);
                }
#endif
                base = timeout;
#if DEPURAR_TIEMPOS
            });
#endif
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

    try {
        switch(id) {
            case EVS_INICIAR_CONSTRUCCION:
                DEPURAR_TIEMPO_EVENTO("iniciar_construccion_edificio", {
                    modelo->iniciar_construccion_edificio(
                        jugador, 
                        evento.at("clase")
                    );
                });
                break;
            
            case EVS_CANCELAR_CONSTRUCCION:
                DEPURAR_TIEMPO_EVENTO("cancelar_construccion_edificio", {
                    modelo->cancelar_construccion_edificio(
                        jugador, 
                        evento.at("clase")
                    );
                });
                break;
            
            case EVS_UBICAR_EDIFICIO: 
                DEPURAR_TIEMPO_EVENTO("ubicar_edificio", {
                    modelo->ubicar_edificio(
                        jugador, 
                        evento.at("celda").get<std::vector<int>>().at(0),
                        evento.at("celda").get<std::vector<int>>().at(1),
                        evento.at("clase")
                    );
                });
                break;
            
            case EVS_VENDER_EDIFICIO:
                DEPURAR_TIEMPO_EVENTO("vender_edificio", {
                    modelo->vender_edificio(
                        jugador,
                        evento.at("id_edificio")
                    );
                });
                break;

            case EVS_INICIAR_ENTRENAMIENTO:
                DEPURAR_TIEMPO_EVENTO("iniciar_entrenamiento_tropa", {
                    modelo->iniciar_entrenamiento_tropa(
                        jugador, 
                        evento.at("clase")
                    );
                });
                break;
            
            case EVS_CANCELAR_ENTRENAMIENTO:
                DEPURAR_TIEMPO_EVENTO("cancelar_entrenamiento_tropa", {
                    modelo->cancelar_entrenamiento_tropa(
                        jugador, 
                        evento.at("clase")
                    );
                });
                break;
            
            case EVS_MOVER_TROPAS: 
                DEPURAR_TIEMPO_EVENTO("mover_tropas", {
                    modelo->mover_tropas(
                        jugador,
                        evento.at("ids_tropa").get<std::unordered_set<int>>(),
                        evento.at("posicion").get<std::vector<int>>().at(0),
                        evento.at("posicion").get<std::vector<int>>().at(1)
                    );
                });
                break;

            case EVS_ATACAR_TROPA:
                DEPURAR_TIEMPO_EVENTO("atacar", {
                    modelo->atacar(
                        jugador,
                        evento.at("ids_tropa").get<std::unordered_set<int>>(),
                        evento.at("id_atacado")
                    );
                });
                break;
            
            case EVS_COSECHADORA_INDICAR_ESPECIA:
                DEPURAR_TIEMPO_EVENTO("indicar_especia_cosechadora", {
                    modelo->indicar_especia_cosechadora(
                        jugador,
                        evento.at("ids_tropa").get<std::unordered_set<int>>(),
                        evento.at("celda").get<std::vector<int>>().at(0),
                        evento.at("celda").get<std::vector<int>>().at(1)
                    );
                });
                break;
            
            default:
                throw std::runtime_error("actualizar_modelo: Evento desconocido");
        }
    } catch(const std::exception& e) {
        std::cout << "Explosión controlada: " << e.what() << std::endl;
    } // Dejar que otras excepciones asciendan
}

} // namespace servidor
