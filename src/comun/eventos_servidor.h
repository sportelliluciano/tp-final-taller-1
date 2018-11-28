#ifndef _EVENTOS_SERVIDOR_H_
#define _EVENTOS_SERVIDOR_H_



/**
 * \brief IDs de los eventos enviados desde el cliente hacia el servidor.
 */
typedef enum {
    EVS_INICIAR_CONSTRUCCION,    // (clase)
    EVS_CANCELAR_CONSTRUCCION,   // (clase)
    EVS_UBICAR_EDIFICIO,         // (clase, celda)
    EVS_VENDER_EDIFICIO,         // (id)
    
    EVS_INICIAR_ENTRENAMIENTO,   // (clase)
    EVS_CANCELAR_ENTRENAMIENTO,  // (clase)

    EVS_MOVER_TROPAS,            // (ids, posicion)
    EVS_ATACAR_TROPA,            // (ids, id_atacado)

    EVS_COSECHADORA_INDICAR_ESPECIA, // (id, celda)
} evento_servidor_t;



#endif // _EVENTOS_SERVIDOR_H_
