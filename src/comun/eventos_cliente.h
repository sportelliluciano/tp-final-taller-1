#ifndef _EVENTOS_CLIENTE_H_
#define _EVENTOS_CLIENTE_H_



/**
 * \brief IDs de los eventos enviados desde el cliente hacia el servidor.
 */
typedef enum {
    // Infraestructura
    EVC_INICIAR_CONSTRUCCION,     // (clase, tiempo)
    EVC_SINCRONIZAR_CONSTRUCCION, // (clase, tiempo)
    EVC_SET_VELOCIDAD_CC,         // (velocidad)
    EVC_ACTUALIZAR_COLA_CC,       // (clase, cantidad)
    EVC_CREAR_EDIFICIO,           // (id, clase, posicion, id_jugador)
    EVC_DESTRUIR_EDIFICIO,        // (id)
    EVC_AGREGAR_EDIFICIO,         // (id, clase, posicion, jugador, vida)
    EVC_ELIMINAR_EDIFICIO,        // (id)
    
    // Ejército
    EVC_ENTRENAR_TROPA,           // (clase, tiempo)
    EVC_SINCRONIZAR_ENTRENAMIENTO, // (clase, tiempo)
    EVC_ACTUALIZAR_COLA_EE,       // (clase, cantidad)
    EVC_CREAR_TROPA,              // (id, clase, posicion_inicial, id_jugador)
    EVC_MOVER_TROPA,              // (id, camino)
    EVC_SINCRONIZAR_TROPA,        // (id, posición actual)
    EVC_ATACAR,                   // (id_atacante, id_victima, nueva_vida)
    EVC_DESTRUIR_TROPA,           // (id)

    // Entorno
    EVC_MOSTRAR_GUSANO,           // (posicion)
    EVC_ELIMINAR_ESPECIA,         // (posicion)
    EVC_COSECHADORA_DESCARGAR,    // (id)

    // Jugador
    EVC_ACTUALIZAR_DINERO,        // (nuevo_dinero, nuevo_maximo)
    EVC_ACTUALIZAR_ENERGIA,       // (nueva_energia, nuevo_maximo)

    // Configuración / Misc
    EVC_INICIALIZAR,
    EVC_JUEGO_INICIANDO,          // (id)
    EVC_CREAR_JUGADOR,            // (id, nombre, casa)
    EVC_JUGADOR_LISTO,            // (id)
    EVC_JUEGO_TERMINADO           // (id_ganador)
} evento_cliente_t;



#endif // _EVENTOS_CLIENTE_H_
