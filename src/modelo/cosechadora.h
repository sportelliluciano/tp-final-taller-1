#ifndef _COSECHADORA_H_
#define _COSECHADORA_H_

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/unidad_base.h"
#include "modelo/jugador.h"
#include "comun/i_jugador.h"

#define ESPERA_CARGA 20000 //20S
#define ESPERA_DESCARGA 150000//5MIN(300000)

namespace modelo {
/**
 * \brief Unidad Cosechadora. 
 * 
 * La cosechadora es la unidad encargada de la carga y descrga
 * de especia(que representa plata) automaticamnete.
 * Terreno: puntero al terreno(unica instancia) del juego.
 * Operando: booleano que representa el estado de la cosechadora.
 * cargando(true) o descargando(false) especia.
 * Id_propietario: identificador del jugador que posee
 * la instancia de cosechadora.
 * Comunicacion_jugador: puntero al jugador poseedor de la cosechadora.
 */
class Cosechadora:public Unidad {
    private:
    Terreno* terreno_;
    bool operando_ = false;
    int tiempo_descarga = ESPERA_CARGA;
    bool camino_especia = true;
    //bool llegue = false;
    int id_propietario;
    Jugador* comunicacion_jugador;

    public:
    /**
    * \brief Constructor.
    */
    Cosechadora(int id,int pos_x,int pos_y, UnidadBase& unidad_base,
        Terreno* terreno,int id_propietario,Jugador* comunicacion_jugador);
    ~Cosechadora();
    /**
    * \brief Actualiza la posicion de la cosechadora.
    * Actualiza la posicion paso a paso de la cosechadora mientras esta se
    * esta moviendo.
    * Se encarga de redirigir la cosechadora a donde corresponda en caso
    * de ser necesario.
    */
    bool actualizar_posicion(int dt,Terreno* terreno);
    /**
     * \brief True=camino a la especia, false=camino a la refineria.
     */
    bool camino_a_especia();
    /**
     * \brief True=cargando o descargando especia, false=en movimiento.
     */
    bool operando();
    /**
     * \brief actualizancion temporal del proceso de carga y descarga..
     */
    void operar(int ds);
    
    int obtener_id_jugador();
};

} // namespace modelo

#endif //_COSECHADORA_H_
