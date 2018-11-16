#include "cliente/modelo/tropa.h"

#include "cliente/video/log.h"
#include "cliente/video/ventana.h"

#define SPRITE_BASE 206
#define THRESHOLD_SYNC_CAMINO 10

namespace cliente {

Tropa::Tropa(const nlohmann::json& data) {
    clase = data.at("id");
    sprite_boton = data.at("sprite_boton");
    int sprite_base = data.at("sprite_base");
    for (int i=0;i<N_SPRITES;i++) {
        std::vector<Sprite> cuadros_caminando, cuadros_disparando;
        for (int j=0; j<7; j++) {
            cuadros_caminando.push_back(Sprite(SPRITE_BASE + j*N_SPRITES + i));
            cuadros_disparando.push_back(Sprite(SPRITE_BASE + (j+7)*N_SPRITES + i));
        }
        sprites_caminando[i] = SpriteAnimado(cuadros_caminando, 10);
        sprites_caminando[i].configurar_repeticion(true);
        sprites_caminando[i].set_centrado(true);
        sprites_parado[i] = SpriteAnimado(SPRITE_BASE + i, SPRITE_BASE + i, 1);
        sprites_parado[i].configurar_repeticion(true);
        sprites_parado[i].set_centrado(true);
        sprites_disparando[i] = SpriteAnimado(cuadros_disparando);
        sprites_disparando[i].set_centrado(true);
        sprites_disparando[i].configurar_repeticion(true);
    }

    fx_actual = x_destino = x_actual = 0;
    fy_actual = y_destino = y_actual = 0;
    id_tropa = -1;
}

void Tropa::inicializar(int id, const Posicion& posicion, int vida_, 
    int id_propietario_)
{
    fx_actual = x_destino = x_actual = posicion.x;
    fy_actual = y_destino = y_actual = posicion.y;
    id_tropa = id;
    vida = vida_;
    id_propietario = id_propietario_;
}

SpriteAnimado& Tropa::obtener_sprite() {
    if (esta_moviendo()) {
        return sprites_caminando[posicion_sprite];
    } else if (esta_disparando()) {
        return sprites_disparando[posicion_sprite];
    }
    return sprites_parado[posicion_sprite];
}

void Tropa::renderizar(Ventana& ventana, int x, int y) {
    SpriteAnimado& sprite_tropa = obtener_sprite();
    
    if (esta_marcada) {
        const Textura& marca = ventana
            .obtener_administrador_texturas()
            .cargar_imagen("./assets/nuevos/unidad-seleccionada.png");
        
        marca.renderizar(x - marca.obtener_ancho() / 2, y - marca.obtener_alto() / 2);
    }

    sprite_tropa.renderizar(ventana, x, y);
}

static int calcular_posicion_sprite(int vx, int vy) {
    switch(vx) {
        case -1:
        switch(vy) {
            case -1:
                return 7; //28;
            case  0:
                return 6; //24;
            case  1:
                return 5; //20;
        }

        case  0:
        switch(vy) {
            case -1:
                return 0; // 0; 
            case  0:
                log_advertencia("Se pidió calcular dirección de sprite para una tropa inmóvil", 0);
                return 0;
            case  1:
                return 4; //16;  
        }

        case  1:
        switch(vy) {
            case -1:
                return 1; //4;
            case  0:
                return 2; //8;
            case  1:
                return 3; //12;
        }
    }

    log_advertencia("ERROR en calcular_pos_sprite", 0);
    return 0;
}

void Tropa::actualizar(int dt_ms) {
    if (!esta_moviendo())
        return;

    float vx = 0, vy = 0;

    if (x_destino != x_actual)
        vx = (x_destino - x_actual) / abs(x_destino - x_actual);
    if (y_destino != y_actual)
        vy = (y_destino - y_actual) / abs(y_destino - y_actual);

    posicion_sprite = calcular_posicion_sprite(vx, vy);

    vx = vx / sqrt(vx*vx + vy*vy);
    vy = vy / sqrt(vx*vx + vy*vy);

    vx *= 0.4 / 15;
    vy *= 0.4 / 15;

    float dx = vx * dt_ms,
          dy = vy * dt_ms;

    if (abs(dx) < abs(x_destino - fx_actual))
        fx_actual += dx;
    else
        fx_actual = x_destino;
        
    if (abs(dy) < abs(y_destino - fy_actual))
        fy_actual += dy;
    else
        fy_actual = y_destino;

    x_actual = round(fx_actual);
    y_actual = round(fy_actual);
}

int Tropa::obtener_id() const {
    return id_tropa;
}

const std::string& Tropa::obtener_clase() const {
    return clase;
}

int Tropa::obtener_sprite_boton() const {
    return sprite_boton;
}

int Tropa::obtener_x() const {
    return x_actual;
}

int Tropa::obtener_y() const {
    return y_actual;
}

bool Tropa::esta_moviendo() const {
    return (x_destino != x_actual) || (y_destino != y_actual);
}

void Tropa::set_esta_disparando(bool disparando) {
    b_esta_disparando = disparando;
}

bool Tropa::esta_disparando() const {
    return b_esta_disparando;
}

void Tropa::caminar_hacia(int x_dest, int y_dest) {
    x_destino = x_dest;
    y_destino = y_dest;

    // TODO
    log_depuracion("Caminando hacia (%.2f, %.2f)", x_destino, y_destino);
}

void Tropa::seguir_camino(const std::vector<std::pair<int, int>>& camino) {
    camino_actual = camino;
    paso_actual = 1;
    
    // Iniciar la caminata.
    sync_camino(camino_actual[0].first, 
        camino_actual[0].second);
}

void Tropa::sync_camino(int x, int y) {
    /*** Chequear posiciones -- dbg ***/
    float mse = ((x_actual - x) * (x_actual - x)) + (y_actual - y) * (y_actual - y);
    if (mse > THRESHOLD_SYNC_CAMINO) {
        log_advertencia("El MSE entre posiciones es >%d [%.2f]", mse,
            THRESHOLD_SYNC_CAMINO);
    }
    
    /******* TODO: Eliminar esto ******/
    log_depuracion("(%d, %d) <> (%d, %d)", x_actual, y_actual, x, y);
    fx_actual = x_actual = x;
    fy_actual = y_actual = y;

    if (paso_actual < camino_actual.size()) {
        caminar_hacia(camino_actual[paso_actual].first, 
            camino_actual[paso_actual].second);
        paso_actual++;
    }
}

void Tropa::set_vida(int nueva_vida) {
    vida = nueva_vida;
}

void Tropa::marcar() {
    esta_marcada = true;
}

void Tropa::desmarcar() {
    esta_marcada = false;
}

} // namespace cliente
