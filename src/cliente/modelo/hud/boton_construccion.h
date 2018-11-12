#ifndef _BOTON_CONSTRUCCION_H_
#define _BOTON_CONSTRUCCION_H_

#include <functional>
#include <string>

#include "cliente/modelo/hud/boton_temporizado.h"
#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/servidor.h"

namespace cliente {

class BotonConstruccion : public BotonTemporizado {
public:
    BotonConstruccion(Infraestructura& infraestructura_, 
        const std::string& clase, Servidor& servidor_, Tostador& tostador_);
    
    bool mouse_click_izquierdo(int x, int y) override;
    bool mouse_click_derecho(int x, int y) override;
    void en_ubicar_nuevo_edificio(std::function<void(void)> callback);
    void pre_renderizar(Ventana& ventana, int x, int y) override;

private:
    Infraestructura& infraestructura;
    std::string clase;
    Servidor& servidor;
    Tostador& tostador;
    std::function<void(void)> cb_ubicar_edificio;
};

} // namespace cliente

#endif // _BOTON_CONSTRUCCION_H_
