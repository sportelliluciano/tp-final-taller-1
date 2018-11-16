#include "cliente/modelo/sprite_animado.h"

#include <vector>

#include "cliente/modelo/sprite.h"
#include "cliente/video/log.h"
#include "cliente/video/ventana.h"

namespace cliente {

SpriteAnimado::SpriteAnimado(
    int sprite_inicial, int sprite_final, int frame_rate) 
{
    for (int i=sprite_inicial; i <= sprite_final; i++) {
        cuadros.emplace_back(Sprite(i, 0, 0));
    }
    ms_por_cuadro = 1000 / frame_rate;
    terminado = false;
}

SpriteAnimado::SpriteAnimado(
    const std::vector<Sprite>& cuadros_, int frame_rate) 
{
    cuadros = cuadros_;
    ms_por_cuadro = 1000 / frame_rate;
    terminado = (cuadros.size() == 0);
}

void SpriteAnimado::desplazar_cuadro(int id_cuadro, int dx, int dy) {
    if ( (id_cuadro < 0) || (id_cuadro >= cuadros.size()) )
        throw std::runtime_error("SpriteAnimado: El cuadro no existe");
    
    cuadros[id_cuadro].desplazar(dx, dy);
}

void SpriteAnimado::renderizar(Ventana& ventana, int x, int y) {
    if (terminado)
        return;
    
    if (cuadro_actual == cuadros.size())
        throw std::runtime_error("SpriteAnimado: Assertion failed en renderizar");
    
    int pos_x = x;
    int pos_y = y;

    if (centrar) {
        pos_x -= cuadros[cuadro_actual].obtener_ancho(ventana) / 2;
        pos_y -= cuadros[cuadro_actual].obtener_alto(ventana) / 2;
    }

    cuadros[cuadro_actual].renderizar(ventana, pos_x, pos_y);

    if (ventana.obtener_ms() > ts_ultimo_cuadro + ms_por_cuadro) {
        ts_ultimo_cuadro = ventana.obtener_ms();
        cuadro_actual++;

        if (cuadro_actual == cuadros.size()) {
            if (!loop)
                terminado = true;
            else
                cuadro_actual = 0;
        }
    }
}

int SpriteAnimado::obtener_alto(Ventana& ventana) {
    return cuadros[cuadro_actual].obtener_textura(ventana).obtener_alto();
}

int SpriteAnimado::obtener_ancho(Ventana& ventana) {
    return cuadros[cuadro_actual].obtener_textura(ventana).obtener_ancho();
}

void SpriteAnimado::configurar_repeticion(bool repetir_indefinidamente) {
    loop = repetir_indefinidamente;
    if (repetir_indefinidamente) 
        terminado = false;
}

void SpriteAnimado::reiniciar() {
    cuadro_actual = 0;
    terminado = false;
}

void SpriteAnimado::set_centrado(bool habilitar) {
    centrar = habilitar;
}

bool SpriteAnimado::finalizado() const {
    return terminado;
}

} // namespace cliente
