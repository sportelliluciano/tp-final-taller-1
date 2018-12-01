#ifndef _DISPARO_H_
#define _DISPARO_H_

#include "cliente/modelo/disparo.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Disparo {
public:
    /**
     * \brief Inicia el disparo desde la posición inicial.
     */
    virtual void iniciar(int x_origen, int y_origen, int x_destino, 
        int y_destino) = 0;

    /**
     * \brief Actualiza el destino de los disparos. 
     * 
     * Escencialmente este método hace que el disparo siga al objetivo.
     */
    virtual void actualizar_destino(int x_destino, int y_destino) = 0;

    /**
     * \brief Detiene los disparos.
     * 
     * Este método espera a que el disparo en curso colisione con el objetivo
     * y luego detiene los disparos.
     */
    virtual void detener() = 0;

    /**
     * \brief Actualiza la posición del disparo.
     */
    virtual void actualizar(int dt_ms) = 0;

    /**
     * \brief Renderiza el disparo en la ventana.
     */
    virtual void renderizar(Ventana& ventana, Camara& camara) = 0;

    virtual ~Disparo() { }
};

} // namespace cliente

#endif // _DISPARO_H_
