#ifndef _DISPARO_PROYECTIL_H_
#define _DISPARO_PROYECTIL_H_

#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Proyectil que sigue al destino actualizando la velocidad según
 *        corresponda.
 */
class DisparoProyectil {
public:
    DisparoProyectil(float frecuencia_disparo);

    /**
     * \brief Inicia el disparo desde la posición inicial.
     */
    virtual void iniciar(int x_origen, int y_origen, int x_destino, 
        int y_destino);

    /**
     * \brief Actualiza el destino de los disparos. 
     * 
     * Esencialmente este método hace que el disparo siga al objetivo.
     */
    virtual void actualizar_destino(int x_destino, int y_destino);

    /**
     * \brief Detiene los disparos.
     * 
     * Este método espera a que el disparo en curso colisione con el objetivo
     * y luego detiene los disparos.
     */
    virtual void detener();

    /**
     * \brief Actualiza la posición del disparo.
     */
    virtual void actualizar(int dt_ms);

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) = 0;

    virtual ~DisparoProyectil() { }

protected:
    bool esta_explotando = false, esta_activo = false, 
        esta_viajando = false;

    const float periodo;
    float velocidad;

    int tiempo_para_impacto;
    int orientacion;

    Posicion origen;
    Posicion pos_actual;
    float fx_actual, fy_actual;
    Posicion pos_victima;

    void actualizar_movimiento(int dt_ms);
    void actualizar_orientacion();
};

} // namespace cliente

#endif // _DISPARO_PROYECTIL_H_
