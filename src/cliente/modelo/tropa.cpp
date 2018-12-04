#include "cliente/modelo/tropa.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "cliente/config.h"

#include "cliente/modelo/disparo_misil.h"
#include "cliente/modelo/disparo_plasma.h"
#include "cliente/modelo/disparo_ondas.h"
#include "cliente/modelo/hud/barra_vida.h"
#include "comun/log.h"
#include "cliente/video/ventana.h"
#include "cliente/sonido/sonido.h"

#define THRESHOLD_SYNC_CAMINO 10

#define CONSTANTE_VELOCIDAD ((0.4 / 15) / 16)

/**
 * Tropas
 * SPRITE_BASE +
 *  8k + h, h = [0 .. 7]: 
 *    h: Posicionamiento (0 = N, 1 = NE, 2 = E, 3 = SE, 4 = S, 5 = SO, 6 = O, 
 *                        7 = NO)
 *    k: [ 0 ..  6]: Animación caminar
 *    k: [ 7 .. 12]: Animación disparar
 *  [176 .. 180]: Animación fallecer
 * 
 * Vehículos
 * SPRITE_BASE +
 *  [0 .. 31]: Posicionamiento (0 = N, 4 = NE, 8 = E, 12 = SE, 16 = S, 20 = SO,
 *                              24 = O, 28 = NO)
 *             Posiciones intermedias: orientaciones intermedias
 */

/**
 * Disparos:
 * Misil: 3336 + 32k + h, k = [0 .. 4], h = [0 .. 31]
 * 
 * k: ¿Frame de animación?
 * h: Orientación
 * 
 * Explosión misil: 3634 + k, k = [0 .. 4]
 * 
 * Plasma: 3496 + k, k = [0 .. 31]
 * k: Frame de animación
 * 
 * Explosión plasma: 3939 + k, k = [0 .. 34]
 * k: Frame de animación
 * 
 * Onda de sonido: 3884 + k, k = [0 .. 14]
 */

#define JSON_CLAVE_CLASE           "id"
#define JSON_CLAVE_SPRITE_BOTON    "sprite_boton"
#define JSON_CLAVE_SPRITE_BASE     "sprite_base"
#define JSON_CLAVE_TIPO            "tipo"
#define JSON_CLAVE_VELOCIDAD       "velocidad"
#define JSON_CLAVE_REQUERIMIENTO   "requerimiento"
#define JSON_CLAVE_CASA            "casa"
#define JSON_CLAVE_NOMBRE          "nombre"
#define JSON_CLAVE_DESCRIPCION     "descripcion"
#define JSON_CLAVE_COSTO           "costo"
#define JSON_CLAVE_T_ENTRENAMIENTO "tiempo de entrenamiento"
#define JSON_CLAVE_VIDA_MAXIMA     "vida"
#define JSON_CLAVE_METADATA        "metadata"
#define JSON_CLAVE_ARMAS           "id_arma"

#define JSON_VALOR_TIPO_VEHICULO   "vehiculo"

#define ID_ARMA_MISIL "lanza_misiles"
#define ID_ARMA_PLASMA "cañon_plasma"
#define ID_ARMA_ONDAS "ondas_sonido"

namespace cliente {

Tropa::Tropa(const nlohmann::json& data) {
    clase = data.at(JSON_CLAVE_CLASE);
    sprite_boton = data.at(JSON_CLAVE_SPRITE_BOTON);
    es_vehiculo = (data.at(JSON_CLAVE_TIPO) == JSON_VALOR_TIPO_VEHICULO);
    velocidad = data.at(JSON_CLAVE_VELOCIDAD);
    requerimientos = 
        data.at(JSON_CLAVE_REQUERIMIENTO).get<std::vector<std::string>>();
    casas_habilitadas = data.at(JSON_CLAVE_CASA).get<std::unordered_set<std::string>>();
    nombre = data.at(JSON_CLAVE_NOMBRE);
    descripcion = data.at(JSON_CLAVE_DESCRIPCION);
    costo = data.at(JSON_CLAVE_COSTO);
    tiempo_entrenamiento = data.at(JSON_CLAVE_T_ENTRENAMIENTO);
    vida = vida_maxima = data.at(JSON_CLAVE_VIDA_MAXIMA);
    
    int sprite_base = data.at(JSON_CLAVE_SPRITE_BASE);
    
    orientacion_sprite = nueva_orientacion_sprite = 0;
    fx_actual = pos_destino.x = pos_actual.x = 0;
    fy_actual = pos_destino.y = pos_actual.y = 0;
    id_tropa = -1;

    std::unordered_map<std::string, std::string> meta = 
        data.at(JSON_CLAVE_METADATA)
            .get<std::unordered_map<std::string, std::string>>();

    for (auto& it : meta) {
        metadata.push_back({it.first, it.second});
    }

    if (es_vehiculo) {
        cargar_sprites_vehiculo(sprite_base);
    } else {
        cargar_sprites_tropa(sprite_base);
    }

    for (const std::string& id_arma : data.at(JSON_CLAVE_ARMAS)) {
        if ((id_arma == ID_ARMA_MISIL) || (id_arma == ID_ARMA_PLASMA) ||
            (id_arma == ID_ARMA_ONDAS))
        {
            id_disparo = id_arma;
            break;
        }
    }
}

void Tropa::cargar_sprites_vehiculo(int sprite_base) {
    for (int i=0;i<N_SPRITES_VEHICULO;i++) {
        sprites_vehiculo[i] = SpriteAnimado({Sprite(sprite_base + i)}, 1);
        sprites_vehiculo[i].configurar_repeticion(true);
        sprites_vehiculo[i].set_centrado(true);
    }
}

void Tropa::cargar_sprites_tropa(int sprite_base) {
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

void Tropa::inicializar(int id, const Posicion& posicion, int vida_, 
    int id_propietario_)
{
    if (id_disparo == ID_ARMA_MISIL)
        disparo = new DisparoMisil();
    else if (id_disparo == ID_ARMA_PLASMA)
        disparo = new DisparoPlasma();
    else if (id_disparo == ID_ARMA_ONDAS)
        disparo = new DisparoOndas();
    
    fx_actual = pos_destino.x = pos_actual.x = posicion.x;
    fy_actual = pos_destino.y = pos_actual.y = posicion.y;
    id_tropa = id;
    if (vida != -1)
        vida = vida_;
    id_propietario = id_propietario_;
}

SpriteAnimado& Tropa::obtener_sprite() {
    if (es_vehiculo) {
        return sprites_vehiculo[orientacion_sprite];
    } else if (esta_moviendo()) {
        return sprites_caminando[orientacion_sprite];
    } else if (esta_disparando()) {
        return sprites_disparando[orientacion_sprite];
    }
    return sprites_parado[orientacion_sprite];
}

void Tropa::renderizar(Ventana& ventana, Camara& camara) {
    Posicion visual = camara.traducir_a_visual(pos_actual);
    SpriteAnimado& sprite_tropa = obtener_sprite();
    
    if (esta_marcada) {
        const Textura& marca = ventana
            .obtener_administrador_texturas()
            .cargar_imagen(RUTA_IMAGENES "/unidad-seleccionada.png");
        
        marca.renderizar(visual.x - marca.obtener_ancho() / 2, 
            visual.y);
        if (paso_actual < camino_actual.size()) {
            int trasladar_x = camara.obtener_vista().x(),
                trasladar_y = camara.obtener_vista().y();
            ventana.dibujar_poligonal(camino_actual, trasladar_x, trasladar_y);
        }
    }

    sprite_tropa.renderizar(ventana, visual.x, visual.y);
    if (disparo && esta_disparando())
        disparo->renderizar(ventana, camara);
    
    barra_vida.set_ancho(sprite_tropa.obtener_ancho(ventana));
    barra_vida.renderizar(ventana, 
        visual.x, 
        visual.y - sprite_tropa.obtener_alto(ventana) / 2,
        vida, vida_maxima);
}

static int calcular_posicion_sprite(int vx, int vy, bool es_vehiculo) {
    if (vx > 0)
        vx = 1;
    else if (vx < 0)
        vx = -1;
    if (vy > 0)
        vy = 1;
    else if (vy < 0)
        vy = -1;
    int pos_sprite = 0;
    switch(vx) {
        case -1:
        switch(vy) {
            case -1: pos_sprite = 7; break;
            case  0: pos_sprite = 6; break;
            case  1: pos_sprite = 5; break;
        }
        break;

        case  0:
        switch(vy) {
            case -1: pos_sprite = 0; break;
            case  0:
                log_advertencia("Se pidió calcular dirección de sprite para"
                    " una tropa inmóvil", 0);
                pos_sprite = 0;
                break;
            case  1: pos_sprite = 4; break;
        }
        break;

        case  1:
        switch(vy) {
            case -1: pos_sprite = 1; break;
            case  0: pos_sprite = 2; break;
            case  1: pos_sprite = 3; break;
        }
        break;
    }

    if (es_vehiculo)
        return pos_sprite * 4;
    
    return pos_sprite;
}

void Tropa::actualizar_ataque(int dt_ms) {
    actualizar_posicion_victima(Posicion(x_atacado, y_atacado));
    if (disparo)
        disparo->actualizar(dt_ms);
}

void Tropa::actualizar_movimiento(int dt_ms) {
    int vx = 0, vy = 0;

    if (pos_destino.x != pos_actual.x)
        vx = pos_destino.x - pos_actual.x;
    if (pos_destino.y != pos_actual.y)
        vy = pos_destino.y - pos_actual.y;

    if ((vx == 0) && (vy == 0))
        return;
    
    float k = CONSTANTE_VELOCIDAD * velocidad / sqrt(vx*vx + vy*vy);
    float veloc_x = k * vx,
          veloc_y = k * vy;

    nueva_orientacion_sprite = 
        calcular_posicion_sprite(vx, vy, es_vehiculo);

    float dx = veloc_x * dt_ms,
          dy = veloc_y * dt_ms;

    if (abs(dx) < abs(pos_destino.x - fx_actual))
        fx_actual += dx;
    else
        fx_actual = pos_destino.x;
        
    if (abs(dy) < abs(pos_destino.y - fy_actual))
        fy_actual += dy;
    else
        fy_actual = pos_destino.y;

    pos_actual.x = round(fx_actual);
    pos_actual.y = round(fy_actual);

    if (orientacion_sprite == nueva_orientacion_sprite)
        return;
    
    last_ms += dt_ms;
    int max_orientacion = (es_vehiculo) ? 32 : 8;
    if (last_ms > (30 * 12.0f / velocidad)) {
        int delta = nueva_orientacion_sprite - orientacion_sprite;
        if (abs(delta) > max_orientacion - abs(delta))
            delta = -delta;
        
        delta = delta / abs(delta);
        
        orientacion_sprite = orientacion_sprite + delta;
        if (orientacion_sprite < 0)
            orientacion_sprite = max_orientacion - 1;
        else if (orientacion_sprite > max_orientacion - 1)
            orientacion_sprite = 0;
        
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

const Posicion& Tropa::obtener_posicion() const {
    return pos_actual;
}

bool Tropa::esta_moviendo() const {
    return !(pos_destino == pos_actual);
}

int Tropa::obtener_propietario() const {
    return id_propietario;
}

void Tropa::atacar(int id_victima, int x_victima, int y_victima) {
    if (esta_disparando() && id_atacado == id_victima)
        return;

    b_esta_disparando = true;
    id_atacado = id_victima;
    if (disparo)
        disparo->iniciar(pos_actual.x, pos_actual.y, x_victima, y_victima);
    
    actualizar_posicion_victima(Posicion(x_victima, y_victima));
}

void Tropa::actualizar_posicion_victima(const Posicion& pos_victima) {
    int x = pos_victima.x, y = pos_victima.y;
    if ((x == x_atacado) && (y == y_atacado))
        return;
    x_atacado = x;
    y_atacado = y;
    orientacion_sprite = calcular_posicion_sprite(x - pos_actual.x, 
        y - pos_actual.y, es_vehiculo);
    if (disparo)
        disparo->actualizar_destino(x, y);
}

int Tropa::obtener_atacado() const {
    return id_atacado;
}

void Tropa::detener_ataque() {
    if (b_esta_disparando) {
        b_esta_disparando = false;
        id_atacado = -1;
        if (disparo)
            disparo->detener();
    }
}

bool Tropa::esta_disparando() const {
    return b_esta_disparando;
}

void Tropa::caminar_hacia(int x_dest, int y_dest) {
    pos_destino.x = x_dest;
    pos_destino.y = y_dest;
    detener_ataque();
}

void Tropa::seguir_camino(const std::vector<std::pair<int, int>>& camino) {
    detener_ataque();
    camino_actual = camino;
    paso_actual = 0;
    // Iniciar la caminata.
    sync_camino(pos_actual.x, pos_actual.y);
}

void Tropa::sync_camino(int x, int y) {
    /*** Chequear posiciones -- dbg ***/
    float mse = ((pos_actual.x - x) * (pos_actual.x - x)) +     
        (pos_actual.y - y) * (pos_actual.y - y);
    if (mse > THRESHOLD_SYNC_CAMINO) {
        log_advertencia("El MSE entre posiciones es >%d [%.2f]", mse,
            THRESHOLD_SYNC_CAMINO);
    }
    
    fx_actual = pos_actual.x = x;
    fy_actual = pos_actual.y = y;

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
    Sonido::reproducir_sonido(SND_UNIDAD_MARCADA);
}

void Tropa::desmarcar() {
    esta_marcada = false;
}

const std::vector<std::string>& Tropa::obtener_requerimientos() const {
    return requerimientos;
}

bool Tropa::casa_puede_entrenar(const std::string& casa) const {
    return casas_habilitadas.count(casa) != 0;
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

Tropa::~Tropa() {
    if (disparo)
        delete disparo;
}

} // namespace cliente
