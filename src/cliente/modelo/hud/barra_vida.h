#ifndef _BARRA_VIDA_H_
#define _BARRA_VIDA_H_

#include "cliente/video/ventana.h"

namespace cliente {

class BarraVida {
public:
    BarraVida();

    void set_ancho(int nuevo_ancho);
    void renderizar(Ventana& ventana, int x, int y, int vida, int maximo);

private:
    int ancho;

    Textura& obtener_textura(Ventana& ventana, int vida, int max);
    Textura& crear_barra_vida(AdministradorTexturas& admin_texturas, 
        const std::string& id_textura, int vida, int max);
};

} // namespace cliente

#endif // _BARRA_VIDA_H_
