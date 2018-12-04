#ifndef _DISPARO_ONDAS_H_
#define _DISPARO_ONDAS_H_

#include "cliente/modelo/disparo_proyectil.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

class DisparoOndas : public DisparoProyectil {
public:
    DisparoOndas();

    virtual void actualizar_destino(int x_destino, int y_destino) override;
    virtual void actualizar(int dt_ms) override;

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) override;

    virtual ~DisparoOndas() { }

private:
    SpriteAnimado ondas;
    SpriteAnimado explosion;

    Posicion nueva_pos_victima;
};

} // namespace cliente

#endif // _DISPARO_ONDAS_H_
