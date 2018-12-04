#ifndef _DISPARO_PLASMA_H_
#define _DISPARO_PLASMA_H_

#include "cliente/modelo/disparo_proyectil.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

class DisparoPlasma : public DisparoProyectil {
public:
    DisparoPlasma();

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) override;

    virtual ~DisparoPlasma() { }

private:
    SpriteAnimado plasma;
    SpriteAnimado explosion;
};

} // namespace cliente

#endif // _DISPARO_PLASMA_H_
