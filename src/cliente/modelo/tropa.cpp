#include "cliente/modelo/tropa.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "cliente/modelo/hud/barra_vida.h"
#include "cliente/video/log.h"
#include "cliente/video/ventana.h"

#define THRESHOLD_SYNC_CAMINO 10

#define CONSTANTE_VELOCIDAD ((0.4 / 15) / 16)

namespace cliente {

Tropa::Tropa(const nlohmann::json& data) {
    clase = data.at("id");
    sprite_boton = data.at("sprite_boton");
    int sprite_base = data.at("sprite_base");
    es_vehiculo = (data.at("tipo") == "vehiculo");
    velocidad = data.at("velocidad");
    requerimientos = data.at("requerimiento").get<std::vector<std::string>>();
    casas_habilitadas = data.at("casa").get<std::unordered_set<std::string>>();
    nombre = data.at("nombre");
    descripcion = data.at("descripcion");
    costo = data.at("costo");
    tiempo_entrenamiento = data.at("tiempo de entrenamiento");

    for (auto& it : data.at("metadata").get<std::unordered_map<std::string, std::string>>()) {
        metadata.push_back({it.first, it.second});
    }

    if (es_vehiculo) {
        for (int i=0;i<N_SPRITES*4;i++) {
            sprites_vehiculo[i] = SpriteAnimado({Sprite(sprite_base + i)}, 1);
            sprites_vehiculo[i].configurar_repeticion(true);
            sprites_vehiculo[i].set_centrado(true);
        }
    } else {
        for (int i=0;i<N_SPRITES;i++) {
            std::vector<Sprite> cuadros_caminando, cuadros_disparando;
            for (int j=0; j<7; j++) {
                cuadros_caminando.push_back(Sprite(sprite_base + j*N_SPRITES + i));
            }
            for (int j=0; j<5; j++) {
                cuadros_disparando.push_back(Sprite(sprite_base + (j+7)*N_SPRITES + i));
            }
            sprites_caminando[i] = SpriteAnimado(cuadros_caminando, 5);
            sprites_caminando[i].configurar_repeticion(true);
            sprites_caminando[i].set_centrado(true);
            sprites_parado[i] = SpriteAnimado(sprite_base + i, sprite_base + i, 1);
            sprites_parado[i].configurar_repeticion(true);
            sprites_parado[i].set_centrado(true);
            sprites_disparando[i] = SpriteAnimado(cuadros_disparando, 10);
            sprites_disparando[i].set_centrado(true);
            sprites_disparando[i].configurar_repeticion(true);
        }
    }

    posicion_sprite = nueva_pos_sprite = 0;

    fx_actual = x_destino = x_actual = 0;
    fy_actual = y_destino = y_actual = 0;
    id_tropa = -1;   
    vida = 50;
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
    if (es_vehiculo) {
        return sprites_vehiculo[posicion_sprite];
    } else if (esta_moviendo()) {
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
    barra_vida.renderizar(ventana, 
        x - sprite_tropa.obtener_ancho(ventana) / 2, 
        y - sprite_tropa.obtener_alto(ventana) / 2,
        vida);
}

static int calcular_posicion_sprite(float veloc_x, float veloc_y, 
    bool es_vehiculo) 
{
    int vx = std::round(veloc_x);
    int vy = std::round(veloc_y);
    int pos_sprite = 0;
    switch(vx) {
        case -1:
        switch(vy) {
            case -1:
                pos_sprite = 7; //28;
                break;
            case  0:
                pos_sprite = 6; //24;
                break;
            case  1:
                pos_sprite = 5; //20;
                break;
        }

        case  0:
        switch(vy) {
            case -1:
                pos_sprite = 0; // 0; 
                break;
            case  0:
                log_advertencia("Se pidió calcular dirección de sprite para"
                    " una tropa inmóvil", 0);
                pos_sprite = 0;
                break;
            case  1:
                pos_sprite = 4; //16;  
                break;
        }

        case  1:
        switch(vy) {
            case -1:
                pos_sprite = 1; //4;
                break;
            case  0:
                pos_sprite = 2; //8;
                break;
            case  1:
                pos_sprite = 3; //12;
                break;
        }
    }

    if (es_vehiculo)
        return pos_sprite * 4;
    
    return pos_sprite;
}


void Tropa::actualizar(int dt_ms) {
    if (!esta_moviendo())
        return;

    int vx = 0, vy = 0;

    if (x_destino != x_actual)
        vx = x_destino - x_actual;
    if (y_destino != y_actual)
        vy = y_destino - y_actual;

    if ((vx == 0) && (vy == 0))
        return;
    
    float veloc_x = vx / sqrt(vx*vx + vy*vy),
          veloc_y = vy / sqrt(vx*vx + vy*vy);

    nueva_pos_sprite = calcular_posicion_sprite(veloc_x, veloc_y, es_vehiculo);
    
    veloc_x *= CONSTANTE_VELOCIDAD * velocidad;
    veloc_y *= CONSTANTE_VELOCIDAD * velocidad;

    float dx = veloc_x * dt_ms,
          dy = veloc_y * dt_ms;

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

    // 0 1 2 3 4 5 6 7 8 9 10 11 12 ... 28 29 30 31 [32 33 34 35 36]

    last_ms += dt_ms;
    if ((posicion_sprite != nueva_pos_sprite) && (last_ms > (30 * 12.0f / velocidad))) {
        log_advertencia("Cambio pos sprite: %d; objetivo: %d", posicion_sprite, nueva_pos_sprite);
        if (nueva_pos_sprite - posicion_sprite > 0)
            posicion_sprite = (posicion_sprite + 1) % 32;
        else
            posicion_sprite = ((posicion_sprite - 1) < 0) ? 31:(posicion_sprite - 1);
        
        last_ms = 0;
    }
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

int Tropa::obtener_propietario() const {
    return id_propietario;
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
    log_depuracion("Caminando hacia (%d, %d)", x_destino, y_destino);
}

void Tropa::seguir_camino(const std::vector<std::pair<int, int>>& camino) {
    camino_actual = camino;
    paso_actual = 0;
    
    // Iniciar la caminata.
    sync_camino(x_actual, y_actual);
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

const std::vector<std::string>& Tropa::obtener_requerimientos() const {
    return requerimientos;
}

bool Tropa::casa_puede_entrenar(const std::string& casa) const {
    //return casas_habilitadas.count(casa) != 0;
    return true;
}

const std::string& Tropa::obtener_nombre() const {
    return nombre;
}

const std::string& Tropa::obtener_descripcion() const {
    return descripcion;
}

const std::vector<std::pair<std::string, std::string>>& 
    Tropa::obtener_metadata() const
{
    return metadata;
}

int Tropa::obtener_costo() const {
    return costo;
}

float Tropa::obtener_tiempo_entrenamiento() const {
    return tiempo_entrenamiento;
}


} // namespace cliente
