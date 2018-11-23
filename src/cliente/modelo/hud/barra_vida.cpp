#include "cliente/modelo/hud/barra_vida.h"

#include <sstream>
#include <string>

#include "cliente/video/ventana.h"

#define MIN_ANCHO_BARRA_VIDA 20
#define ALTO_BARRA_VIDA 5
#define BORDE_BARRA_VIDA 1

namespace cliente {

BarraVida::BarraVida() {
    ancho = MIN_ANCHO_BARRA_VIDA;
}

void BarraVida::set_ancho(int nuevo_ancho) {
    if (nuevo_ancho < MIN_ANCHO_BARRA_VIDA)
        ancho = MIN_ANCHO_BARRA_VIDA;
    else
        ancho = nuevo_ancho;
}

void BarraVida::renderizar(Ventana& ventana, int x, int y, int vida, int max) {
    std::stringstream s;
    s << "vida-" << ancho << "-" << vida;

    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura(s.str())) {
        admin_texturas.obtener_textura(s.str())
            .renderizar(x - (ancho / 2), y - ALTO_BARRA_VIDA);
    } else {
        Textura& textura = admin_texturas.crear_textura(
            s.str(), ancho, ALTO_BARRA_VIDA
        );

        int ancho_vida = ancho - 2 * BORDE_BARRA_VIDA;

        if (vida < max) {
            ancho_vida = ancho_vida * (vida / (float)max);
        }

        Rectangulo fondo(0, 0, textura.obtener_ancho(), textura.obtener_alto());
        Rectangulo borde(0, 0, textura.obtener_ancho() - BORDE_BARRA_VIDA, 
            textura.obtener_alto() - BORDE_BARRA_VIDA);
        Rectangulo relleno_vida
            = Rectangulo(BORDE_BARRA_VIDA, BORDE_BARRA_VIDA, 
                ancho_vida, textura.obtener_alto() - 2 * BORDE_BARRA_VIDA);

        textura.rellenar_rectangulo(fondo, 0, 0, 0, 255);
        textura.dibujar_rectangulo(borde, BORDE_BARRA_VIDA, 0, 0, 127, 255);
        textura.rellenar_rectangulo(relleno_vida, 190, 0, 0, 255);
        textura.renderizar(x - (ancho / 2), y - ALTO_BARRA_VIDA);
    }
}

} // namespace cliente
