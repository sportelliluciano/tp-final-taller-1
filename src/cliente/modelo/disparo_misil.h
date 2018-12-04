#ifndef _DISPARO_MISIL_H_
#define _DISPARO_MISIL_H_

#include "cliente/modelo/disparo_proyectil.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

class DisparoMisil : public DisparoProyectil {
public:
    DisparoMisil();

    /**
     * \brief Actualiza el misil.
     */
    void actualizar(int dt_ms);

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) override;

    virtual ~DisparoMisil() { }

private:
    const static int N_SPRITES_MISIL = 32;
    SpriteAnimado misil[N_SPRITES_MISIL];
    SpriteAnimado explosion;

    int orientacion_actual;
};

} // namespace cliente

#endif // _DISPARO_MISIL_H_
