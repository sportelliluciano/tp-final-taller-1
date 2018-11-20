#include "cliente/modelo/hud/barra_vida.h"

#include <sstream>
#include <string>

#include "cliente/video/ventana.h"

#define ANCHO_BARRA_VIDA 20
#define ALTO_BARRA_VIDA 5
#define BORDE_BARRA_VIDA 1

namespace cliente {

void BarraVida::renderizar(Ventana& ventana, int x, int y, int vida) {
    std::stringstream s;
    s << "vida-" << vida;

    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura(s.str())) {
        admin_texturas.obtener_textura(s.str())
            .renderizar(x - (ANCHO_BARRA_VIDA / 2), y - ALTO_BARRA_VIDA);
    } else {
        Textura& textura = admin_texturas.crear_textura(
            s.str(), ANCHO_BARRA_VIDA, ALTO_BARRA_VIDA
        );

        int ancho = ANCHO_BARRA_VIDA - 2 * BORDE_BARRA_VIDA;

        if (vida < 100) {
            ancho = ancho * (vida / 100.0f);
        }

        Rectangulo fondo(0, 0, textura.obtener_ancho(), textura.obtener_alto());
        Rectangulo borde(0, 0, textura.obtener_ancho() - BORDE_BARRA_VIDA, 
            textura.obtener_alto() - BORDE_BARRA_VIDA);
        Rectangulo relleno_vida
            = Rectangulo(BORDE_BARRA_VIDA, BORDE_BARRA_VIDA, 
                ancho, textura.obtener_alto() - 2 * BORDE_BARRA_VIDA);

        textura.rellenar_rectangulo(fondo, 0, 0, 0, 255);
        textura.dibujar_rectangulo(borde, BORDE_BARRA_VIDA, 0, 0, 127, 255);
        textura.rellenar_rectangulo(relleno_vida, 190, 0, 0, 255);
        textura.renderizar(x - (ANCHO_BARRA_VIDA / 2), y - ALTO_BARRA_VIDA);
    }
}

} // namespace cliente
