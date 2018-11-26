#ifndef _DISPARO_MISIL_H_
#define _DISPARO_MISIL_H_

#include "cliente/modelo/disparo.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

class DisparoMisil : public Disparo {
public:

    DisparoMisil();
    /**
     * \brief Inicia el disparo desde la posición inicial hasta la final, 
     *        siguiendo al objetivo mediante interpolación lineal.
     * 
     * La posición de destino no se modificará, pero se utilizará en los
     * subsiguientes llamados a actualizar para seguir al objetivo de ser
     * necesario.
     */
    void disparar(const Posicion& desde, Posicion& hasta);

    /**
     * \brief Actualiza la posición del disparo.
     */
    void actualizar(int dt_ms);

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    void renderizar(Ventana& ventana, Camara& camara);

private:
    const static int N_SPRITES_MISIL = 5;
    SpriteAnimado misil[N_SPRITES_MISIL];

    bool esta_siendo_disparado = false;

    Posicion posicion_actual;
    const Posicion* destino;
};

} // namespace cliente

#endif // _DISPARO_MISIL_H_
