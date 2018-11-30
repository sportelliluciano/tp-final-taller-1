#include "cliente/modelo/edificio.h"

#include <string>
#include <vector>
#include <map>
#include <utility>

#include "libs/json.hpp"

#include "cliente/config.h"
#include "cliente/modelo/celda.h"
#include "cliente/video/ventana.h"

#define SPRITE_CONSTRUCCION_INICIO 3765
#define SPRITE_CONSTRUCCION_FIN 3783

#define SPRITE_DESTRUCCION_INICIO 3686
#define SPRITE_DESTRUCCION_FIN 3700

#define ANCHO_BARRA_VIDA 80

namespace cliente {

Edificio::Edificio(const nlohmann::json& data_edificio) {
    clase = data_edificio.at("id");

    Sprite sprite_base = Sprite::desde_json(data_edificio.at("sprite_base"));
    Sprite sprite_construido = 
        Sprite::desde_json(data_edificio.at("sprite_construido"));
    Sprite sprite_destruido = 
        Sprite::desde_json(data_edificio.at("sprite_destruido"));
    
    sprite = SpriteCompuesto({sprite_construido, sprite_base});
    sprite_roto = SpriteCompuesto({sprite_destruido, sprite_base});

    sprite_construccion = 
        SpriteAnimado(SPRITE_CONSTRUCCION_INICIO, SPRITE_CONSTRUCCION_FIN);
    sprite_construccion.set_centrado(true);
    
    sprite_destruccion = 
        SpriteAnimado(SPRITE_DESTRUCCION_INICIO, SPRITE_DESTRUCCION_FIN);
    sprite_destruccion.set_centrado(true);
    
    sprite_boton = data_edificio.at("sprite_boton");
    nombre = data_edificio.at("nombre");
    ancho = data_edificio.at("dimensiones").at(0);
    alto = data_edificio.at("dimensiones").at(1);
    vida = vida_maxima = data_edificio.at("puntos_estructura");
    nombre = data_edificio.at("nombre");
    descripcion = data_edificio.at("descripcion");
    costo = data_edificio.at("costo");
    tiempo_construccion = data_edificio.at("tiempo_construccion");

    for (auto& it : data_edificio.at("metadata").get<std::map<std::string, std::string>>()) {
        metadata.push_back({it.first, it.second});
    }
}

void Edificio::renderizar(Ventana& ventana, int x_px, int y_px) {
    if (!esta_vivo())
        return;

    if (esta_destruido) {
        sprite_destruccion.renderizar(ventana, x_px, y_px);
        return;
    } else if (!esta_construido) {
        sprite_construccion.renderizar(ventana, x_px, y_px);
        esta_construido = sprite_construccion.finalizado();
    } else {
        const Textura& t = sprite.obtener_textura(ventana);
        sprite.renderizar(ventana, 
            x_px - (t.obtener_ancho() / 2), 
            y_px - (t.obtener_alto() / 2)
        );
        if (marcado) {
            ventana
                .obtener_administrador_texturas()
                .cargar_imagen(RUTA_IMAGENES "/seleccion-edificio.png")
                .renderizar(x_px + sprite.obtener_ancho(ventana) / 2, y_px);
        }
        barra_vida.set_ancho(ANCHO_BARRA_VIDA);
        barra_vida.renderizar(ventana, x_px, y_px - (t.obtener_alto() / 2), 
            vida, vida_maxima);
    }
    
}

void Edificio::inicializar(int id_, int x, int y, int propietario, 
    bool construido, int nueva_vida) 
{
    id_edificio = id_;
    pos_x = x;
    pos_y = y;
    esta_construido = construido;
    esta_destruido = false;
    if (nueva_vida != -1)
        vida = nueva_vida;
    id_jugador = propietario;
}

int Edificio::obtener_id() const {
    return id_edificio;
}

int Edificio::obtener_propietario() const {
    return id_jugador;
}

const std::string& Edificio::obtener_clase() const {
    return clase;
}

int Edificio::obtener_sprite_boton() const {
    return sprite_boton;
}

void Edificio::destruir() {
    esta_destruido = true;
}

int Edificio::obtener_celda_x() const {
    return pos_x;
}

int Edificio::obtener_celda_y() const {
    return pos_y;
}

int Edificio::obtener_ancho_celdas() const {
    return ancho;
}

int Edificio::obtener_alto_celdas() const {
    return alto;
}

bool Edificio::esta_vivo() const {
    return !sprite_destruccion.finalizado();
}

void Edificio::set_vida(int nueva_vida) {
    vida = nueva_vida;
}

void Edificio::marcar() {
    marcado = true;
}

void Edificio::desmarcar() {
    marcado = false;
}

const std::string& Edificio::obtener_nombre() const {
    return nombre;
}

const std::string& Edificio::obtener_descripcion() const {
    return descripcion;
}

const std::vector<std::pair<std::string, std::string>>& 
    Edificio::obtener_metadata() const 
{
    return metadata;
}

int Edificio::obtener_costo() const {
    return costo;
}

float Edificio::obtener_tiempo_construccion() const {
    return tiempo_construccion;
}


} // namespace cliente
