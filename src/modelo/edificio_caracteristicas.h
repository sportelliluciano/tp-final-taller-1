#ifndef _EDIFICIO_CARACTERISTICAS_H_
#define _EDIFICIO_CARACTERISTICAS_H_

#include "modelo/posicion.h"

namespace modelo {
/**
 * \brief EdificioCaracteristicas. 
 * 
 * Encampsula la infromacion de una instancia
 * especifica de edificio.
 * Id: identificador unico dentro del juego.
 * Posicion: Posicion en la que se encuentra el edificio,
 * en caso de la dimension(especificada en EdificioBase) se mayor a 1x1,
 * la posicion indica la posicion noroeste del edificio.
 * Puntos_de_estructura: cantidad de "vida" que tiene cada instancia.
 */
class EdificioCaracteristicas{
    private:
    int id;
    Posicion posicion;
    int puntos_de_estructura;

    public:
    EdificioCaracteristicas();
    EdificioCaracteristicas(int id,int x,int y,
        unsigned int puntos_de_estructura);
    ~EdificioCaracteristicas();
    /**
     * \brief le resta dano a puntos_de_estructura.
     * devuelve los puntos_de_estructura actualizados.
     */
    int reducir_ptos_est(int dano);
    int get_id();
    int get_vida();
    Posicion& get_posicion();
};

} // namespace modelo

#endif //_EDIFICIO_CARACTERISTICAS_H_
