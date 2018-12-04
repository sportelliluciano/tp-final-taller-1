#ifndef GENERADOR_SPRITES_H
#define GENERADOR_SPRITES_H

#include <string>
#include <map>
#include <QPixmap>
#include <vector>
#include "editor/sprite.h"

class GeneradorSprites {
    public:
        /**
         * \brief Constructor de GeneradorSprites.
         * 
         * Inicializo imagen_terrenos.
         */
        GeneradorSprites();
    
        /**
         * \brief Genera todos los sprites de terrenos posible.
         * 
         * Genero todos los Sprite que pueden aparecer en un Mapa. 
         */
        std::map<std::string, Sprite> generar_sprites_posibles();

        /**
         * \brief Genera Sprite inicial o por default de Mapa (arena).
         * 
         * Genero el Sprite de arena utilizado como default por Mapa. 
         */
        Sprite generar_sprite_inicial();

        /**
         * \brief Genera Sprite.
         * 
         * Genero un Sprite a partir de los tiles descriptos por el vector 
         * pos_tiles (posicion en imagen_terrenos).
         */
        Sprite generar_sprite(std::string& id, int tipo, 
            std::vector<uint32_t>& pos_tiles);

        /**
         * \brief Destructor de GeneradorSprites.
         * 
         * Destructor de GeneradorSprites.
         */
        ~GeneradorSprites() {};

    private:
        QPixmap imagen_terrenos;

        /**
         * \brief Genero imagen (QPixmap).
         * 
         * Genero una imagen a partir de los tiles descriptos por el vector 
         * pos_tiles (posicion en imagen_terrenos).
         */
        QPixmap generar_pixmap(std::vector<uint32_t>& pos_tiles);
};

#endif // GENERADOR_SPRITES_H
