#include "editor/generador_sprites.h"
#include "libs/json.hpp"
#include <QPainter>
#include <fstream>
#include "editor/config.h"
using std::vector;
using nlohmann::json;
using std::map;
using std::string;
using std::vector;

GeneradorSprites::GeneradorSprites() {
    this->imagen_terrenos = QPixmap (RUTA_TILES_TERRENO);
}

map<string, Sprite> GeneradorSprites::generar_sprites_posibles() {
    map<string, Sprite> sprites_posibles;

    std::ifstream entrada(RUTA_TERRENOS_JSON);

    json terrenos_json;

    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for (; it != terrenos_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        auto it_sprites = elem["sprites"].begin();
        for (int i = 0; it_sprites != elem["sprites"].end(); ++it_sprites) {
            json tile = *it;

            Sprite sprite;
            sprite.tipo = elem["tipo"];
            vector<uint32_t> pos_tiles = tile["sprites"][i]["pos_tiles"];
            string id = tile["sprites"][i]["id"];

            // genero el pixmap
            QPixmap imagen = generar_pixmap(pos_tiles);

            sprite.imagen = imagen;

            sprites_posibles.emplace(id, sprite);
        }
    }

    return std::move(sprites_posibles);
}

Sprite GeneradorSprites::generar_sprite_inicial() {
    Sprite sprite_inicial;

    // cosas de parseo del json de terrenos.
    std::ifstream entrada(RUTA_TERRENOS_JSON);
    json terrenos_json;
    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    
    ++it;
    const json& elem = *it;

    json tile = *it;
    string id = tile["sprites"][0]["id"];
    int tipo = elem["tipo"];
    vector<uint32_t> pos_tiles = tile["sprites"][0]["pos_tiles"];

    // genero el pixmap
    QPixmap imagen = generar_pixmap(pos_tiles);

    // setteo sprite_inicial y lo devuelvo
    sprite_inicial.id = id;
    sprite_inicial.tipo = tipo;
    sprite_inicial.imagen = imagen;

    return sprite_inicial;
}

Sprite GeneradorSprites::generar_sprite(std::string& id, int tipo, 
    std::vector<uint32_t>& pos_tiles) {
    Sprite sprite;
    
    // genero el pixmap
    QPixmap imagen = generar_pixmap(pos_tiles);

    // setteo sprite y lo devuelvo
    sprite.id = id;
    sprite.tipo = tipo;
    sprite.imagen = imagen;

    return sprite;
}

QPixmap GeneradorSprites::generar_pixmap(std::vector<uint32_t>& pos_tiles) {
    QPixmap label_32_x_32 (32, 32);
    
    // junto los 16 tiles de 8x8 pixeles, cuyas posiciones se encuentran en 
    // pos_tiles y los dibujo en un unico QPixmap de 32x32 pixeles. 
    // pos_x_label y pos_y_label es la posicion de cada sprite de 8x8 dentro
    // del QPixmap de 32x32.
    int pos_x_label = 0;
    int pos_y_label = 0;
    vector<uint32_t>::iterator it_pos_tiles = pos_tiles.begin();
    for (int cont_tiles = 0; it_pos_tiles != pos_tiles.end(); ++it_pos_tiles) {
        // 8 es el tamaño individual de cada sprite (8x8 pixeles).
        // 20 es el ancho del archivo .bmp que contiene todos los sprites 
        // de los terrenos.
        // x e y son las posiciones de cada sprite de 8x8 en el archivo .bmp .
        // los -1 es que arranco a contar desde 1.
        int y = ((*it_pos_tiles - 1) / 20) * 8;
        int x;
        if (*it_pos_tiles < 20) {
            x = (*it_pos_tiles - 1) * 8;
        } else {
            x = ((*it_pos_tiles - 1) % 20) * 8;
        }

        // copio el cuadrado de 8x8 que quiero del .bmp .
        QRect rect(x, y, 8, 8);
        QPixmap cropped = this->imagen_terrenos.copy(rect);
    
        QPainter painter (&label_32_x_32);
        if (cont_tiles < 4) {
            pos_x_label = cont_tiles * 8;
        } else {
             pos_x_label = (cont_tiles % 4) * 8;
        }

        pos_y_label = (cont_tiles / 4) * 8;
        painter.drawPixmap(pos_x_label, pos_y_label, 8, 8, cropped);

        cont_tiles++;
    }

    return label_32_x_32;
}
