#ifndef _SALA_H_
#define _SALA_H_

#include <chrono>
#include <unordered_map>
#include <stdexcept>
#include <mutex>
#include <thread>

#include "libs/json.hpp"

#include "comun/conexion.h"
#include "comun/i_modelo.h"
#include "servidor/cliente.h"
#include "servidor/conexion_jugador.h"
#include "comun/cola_protegida.h"

namespace servidor {

class Sala {
public:
    Sala(const std::string& nombre_, size_t capacidad_maxima, IModelo* juego);

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
     * 
     * La sala eliminará al cliente de la misma con lo cual es seguro eliminar
     * al mismo luego de llamar a este método.
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

    /**
     * \brief Devuelve la cantidad de jugadores conectados a la sala.
     */
    int cantidad_jugadores_conectados();

    /**
     * \brief Setea la casa del cliente.
     */
    bool set_casa_cliente(Cliente& cliente, const std::string& casa);

    /**
     * \brief Envía al cliente la lista de jugadores conectados.
     */
    void listar_jugadores(Cliente& cliente);

    ~Sala();

private:
    std::unique_ptr<IModelo> modelo = nullptr;
    std::mutex lock_modelo;

    std::string nombre;

    std::thread hilo_partida;

    std::unordered_map<int, ConexionJugador*> jugadores;
    std::unordered_map<Cliente*, ConexionJugador> clientes;
    
    bool sala_abierta = true;
    
    int ultimo_id = 0;
    
    bool partida_iniciada = false;

    bool terminar;

    size_t capacidad = 0;

    ColaProtegida<std::pair<IJugador*, nlohmann::json>> cola_eventos;

    void actualizar_modelo(IJugador* jugador, const nlohmann::json& evento);
    
    /**
     * \brief Configura los callbacks de los clientes para que actualicen el
     *        modelo.
     */
    void configurar_recepcion_eventos();
};

} // namespace servidor

#endif // _SALA_H_