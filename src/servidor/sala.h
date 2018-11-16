#ifndef _SALA_H_
#define _SALA_H_

#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <mutex>
#include <thread>

#include "conexion/conexion.h"
#include "conexion/i_modelo.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"

namespace servidor {

class Sala {
public:
    Sala(size_t capacidad_maxima);

    /**
     * \brief Constructor por movimiento
     */
    Sala(Sala&& otro);

    /**
     * \brief Agrega un nuevo jugador a la sala. 
     * 
     * Si la sala está llena se lanzará una excepción.
     */
    void agregar_cliente(Cliente& cliente);
    void eliminar_cliente(Cliente& cliente);

    /**
     * \brief Indica a la sala que un cliente se desconectó.
     */
    void notificar_desconexion(Cliente& cliente);

    /**
     * \brief Inicia la partida
     */
    void jugar();

    /**
     * \brief Devuelve true si se puede unir un cliente a esta sala.
     * 
     * Un cliente no puede unirse si sucede alguna de las siguientes:
     *  - La sala está llena
     *  - La partida ya empezó
     */
    bool puede_unirse() const;

    /**
     * \brief Indica a la sala que el cliente inició la partida.
     * 
     * Cuando todos los clientes en la sala hayan iniciado la partida se 
     * iniciará el juego.
     */
    void iniciar_partida(Cliente& cliente);

    /**
     * \brief Termina abruptamente la partida
     */
    void terminar_partida();

    /**
     * \brief Devuelve la cantidad máxima de jugadores que pueden estar en
     *        la sala.
     */
    size_t obtener_capacidad();

    ~Sala();

private:
    IModelo *modelo = nullptr;
    std::mutex lock_modelo;

    std::thread hilo_partida;

    std::unordered_map<int, ConexionJugador*> jugadores;
    std::unordered_map<Cliente*, ConexionJugador> clientes;
    int ultimo_id = 0;
    
    bool partida_iniciada = false;

    bool terminar;
    const int TICK_MS = 20;

    size_t capacidad = 0;

    void actualizar_modelo(IJugador* jugador, const nlohmann::json& evento);
    
    /**
     * \brief Configura los callbacks de los clientes para que actualicen el
     *        modelo.
     */
    void configurar_recepcion_eventos();
};

} // namespace servidor

#endif // _SALA_H_