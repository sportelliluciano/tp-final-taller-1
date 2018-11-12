#include "cliente/modelo/hud/tostador.h"

#include "cliente/modelo/hud/tostada.h"
#include "cliente/video/widgets/widget.h"
#include "cliente/video/ventana.h"

namespace cliente {

Tostador::Tostador(int x, int y) {
    pos_x = x;
    pos_y = y;
}

int Tostador::obtener_ancho() const {
    return 0;
}

int Tostador::obtener_alto() const {
    return 0;
}

void Tostador::renderizar(Ventana& ventana, int, int) {
    int dy = 0;
    for (auto it=tostadas.begin(); it != tostadas.end();) {
        Tostada& tostada = *it;
        if (tostada.esta_terminada())
            it = tostadas.erase(it);
        else
            ++it;
        tostada.renderizar(ventana, pos_x, pos_y + dy);
        dy += tostada.obtener_alto();
    }
}

void Tostador::hacer_tostada(const std::string& mensaje, int duracion_ms) {
    tostadas.emplace_back(Tostada(mensaje, duracion_ms));
}

} // namespace cliente
