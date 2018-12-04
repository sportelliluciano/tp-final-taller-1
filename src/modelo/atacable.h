#ifndef _ATACABLE_H_
#define _ATACABLE_H_

#include <utility> 

#include "modelo/posicion.h"
/**
 * \brief Interfaz para clases que pueden ser atacadas
 */
class Atacable {
public:
    /**
     * \brief Reduce la vida en cantidad igual a dano.
     */
    virtual int recibir_dano(unsigned int dano) = 0;
    /**
     * \brief Devuelve el id (unico para cada objeto dentro del juego).
     */
    virtual int get_id() = 0;
    /**
     * \brief Devuelve una referencia a la posicion del Atacable.
     * Posicion hace referencia a las coordenadas en celdas dentro del
     * terreno.
     */
    virtual modelo::Posicion& get_posicion() = 0;
    /**
     * \brief Devuelve una referencia a la dimension del Atacable.
     * Entendiendose por dimension a la cantidad de celdas que ocupa
     * en formato altoxancho.
     */
    virtual std::pair<int,int>& get_dimensiones() = 0;
    /**
     * \brief Destructor virtual.
     */
    virtual ~Atacable() { }
};

#endif //_ATACABLE_H_
