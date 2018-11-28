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
     * \brief Inicia el disparo desde la posición inicial.
     */
    virtual void iniciar(int x_origen, int y_origen, int x_destino, 
        int y_destino) override;

    /**
     * \brief Actualiza el destino de los disparos. 
     * 
     * Escencialmente este método hace que el disparo siga al objetivo.
     */
    virtual void actualizar_destino(int x_destino, int y_destino) override;

    /**
     * \brief Detiene los disparos.
     * 
     * Este método espera a que el disparo en curso colisione con el objetivo
     * y luego detiene los disparos.
     */
    virtual void detener() override;

    /**
     * \brief Actualiza la posición del disparo.
     */
    virtual void actualizar(int dt_ms) override;

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) override;

    void actualizar_movimiento(int dt_ms);

private:
    const static int N_SPRITES_MISIL = 32;
    SpriteAnimado misil[N_SPRITES_MISIL];
    SpriteAnimado explosion;

    bool esta_explotando = false, esta_activo = false, 
        esta_viajando = false;

    Posicion origen;
    Posicion pos_actual;
    float fx_actual, fy_actual;
    Posicion pos_victima;
};

} // namespace cliente

#endif // _DISPARO_MISIL_H_
