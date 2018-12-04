#ifndef _ATACABLE_H_
#define _ATACABLE_H_

#include <utility> 

#include "modelo/posicion.h"
/**
 * \brief Interfaz servidor-modelo
 */
class Atacable {
public:
    /**
     * \brief Inicializa el modelo con el mapa, infraestructura y ejército
     *        indicados.
     * 
     * Este método será llamado antes que cualquier otro método.
     */
    virtual int recibir_dano(unsigned int dano) = 0;
    virtual int get_id() = 0;
    virtual modelo::Posicion& get_posicion() = 0;
    virtual std::pair<int,int>& get_dimensiones() = 0;
    /**
     * \brief Destructor virtual.
     */
    virtual ~Atacable() { }
};

#endif 