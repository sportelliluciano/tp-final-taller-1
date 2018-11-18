#ifndef _BOTON_ENTRENAMIENTO_H_
#define _BOTON_ENTRENAMIENTO_H_

#include <functional>
#include <string>

#include "cliente/modelo/hud/boton_temporizado.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/ejercito.h"
#include "cliente/servidor.h"

namespace cliente {

class BotonEntrenamiento : public BotonTemporizado {
public:
    BotonEntrenamiento(Ejercito& ejercito_, 
        const std::string& clase, Servidor& servidor_, Tostador& tostador_);
    
    bool mouse_click_izquierdo(int x, int y) override;
    bool mouse_click_derecho(int x, int y) override;
    void renderizar(Ventana& ventana, int x, int y) override;

private:
    std::string clase;
    Ejercito& ejercito;
    Servidor& servidor;
    Tostador& tostador;
};

} // namespace cliente

#endif // _BOTON_ENTRENAMIENTO_H_
