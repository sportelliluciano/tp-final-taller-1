#include "cliente/modelo/celda.h"

#include <sstream>
#include <string>

#include "cliente/config.h"
#include "cliente/modelo/edificio.h"
#include "cliente/modelo/tropa.h"

#define ANCHO_MOSAICO 8
#define ALTO_MOSAICO 8

#define COLS_MOSAICO 20
#define FILAS_MOSAICO 40

#define RUTA_MOSAICO RUTA_TERRENOS "/d2k_BLOXBASE.bmp"

namespace cliente {

int Celda::uid_celda = 0;

Celda::Celda(tipo_celda_t tipo) {
    tipo_celda = tipo;
    id_celda = uid_celda++;
}

Celda::Celda(tipo_celda_t tipo, const std::vector<std::vector<int>>& sprites,
    size_t x, size_t y)
{
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            sprites_celda[i * 4 + j] = sprites.at(y*4 + i).at(x*4 + j);
        }
    }
    tipo_celda = tipo;
    tiene_sprites = true;
    id_celda = uid_celda++;
}

bool Celda::es_construible() const {
    return (tipo_celda == CELDA_ROCA) && (edificio == nullptr);
}

void Celda::agregar_tropa(Tropa& tropa) {
    tropas.push_back(&tropa);
}

bool Celda::contiene_tropas() const {
    return tropas.size() != 0;
}

const std::list<Tropa*>& Celda::obtener_tropas() const {
    return tropas;
}

void Celda::eliminar_tropa(const Tropa& tropa_eliminada) {
    for (auto it=tropas.begin(); it != tropas.end();) {
        const Tropa* tropa = *it;

        if (tropa == &tropa_eliminada)
            it = tropas.erase(it);
        else
            ++it;
    }
}

void Celda::set_edificio(Edificio& nuevo_edificio) {
    if (contiene_edificio())
        throw std::runtime_error("La celda ya tenía un edificio");
    
    edificio = &nuevo_edificio;
}

bool Celda::contiene_edificio() const {
    return edificio != nullptr;
}

Edificio& Celda::obtener_edificio() {
    if (!contiene_edificio())
        throw std::runtime_error("La celda no contiene ningún edificio");
    
    return *edificio;
}

void Celda::eliminar_edificio(const Edificio& edificio_eliminado) {
    if (edificio != &edificio_eliminado)
        throw std::runtime_error("El edificio a eliminar no coincide");
    
    edificio = nullptr;
}

void Celda::renderizar(Ventana& ventana, int x, int y, Textura& destino) {
    std::stringstream s;
    s << "celda-" << id_celda;
    std::string s_id_celda = s.str();

    if (ventana.obtener_administrador_texturas().contiene_textura(s_id_celda)) {
        ventana
            .obtener_administrador_texturas()
            .obtener_textura(s_id_celda)
            .renderizar(x, y, destino);
        return;
    }

    const Textura& mosaico = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(RUTA_MOSAICO);
    
    Textura& textura_celda = ventana
        .obtener_administrador_texturas()
        .crear_textura(s_id_celda, 32, 32);
    
    if (!tiene_sprites) {
        /**
         * Esto es horrible, pero es la única forma que encontré de que la arena
         * se vea bien.
         * A ser implementado por parte del editor de mapas.
         */
        static std::vector<int> ids_arena = {0, 8, 9, 10, 11, 16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 36, 37, 38, 39, 40, 41, 42, 
            43, 44, 45, 46, 47, 48, 49, 50, 51, 56, 57, 58, 59, 60, 61, 62, 63, 64, 
            65, 66, 68, 69, 70, 71, 72, 76, 77, 78, 79, 80, 81, 82, 83, 84, 100, 
            101, 102, 103, 104};
        
        static std::vector<int> ids_roca = {552, 553, 554, 555, 572, 573, 574, 575};

        int arena = (id_celda * 10001 + x * 1001 + y) % ids_arena.size();
        int roca = (id_celda * 10001 + x * 1001 + y) % ids_roca.size();

        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                int tile_id = 0;
                if (tipo_celda == CELDA_ARENA) {
                    tile_id = ids_arena[arena];
                    arena = (arena + 1) % ids_arena.size();
                } else if (tipo_celda == CELDA_ROCA) {
                    tile_id = ids_roca[roca];
                    roca = (roca + 1) % ids_roca.size();
                } else if (tipo_celda == CELDA_ESPECIA) {
                    tile_id = 799;
                } else if (tipo_celda == CELDA_PRECIPICIO) {
                    tile_id = 1;
                } else if (tipo_celda == CELDA_DUNA) {
                    tile_id = 663;
                }

                int fila_mosaico = tile_id / COLS_MOSAICO;
                int col_mosaico = tile_id % COLS_MOSAICO;

                if (fila_mosaico >= FILAS_MOSAICO)
                    throw std::runtime_error("Número de mosaico inválido");
                
                int y_mosaico = fila_mosaico * ALTO_MOSAICO;
                int x_mosaico = col_mosaico * ANCHO_MOSAICO;
                Rectangulo seccion(x_mosaico, y_mosaico, ANCHO_MOSAICO,
                    ALTO_MOSAICO);
                mosaico.renderizar(
                    i * ANCHO_MOSAICO, j * ALTO_MOSAICO, 
                    seccion, textura_celda);
            }
        }
    } else {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                int tile_id = sprites_celda[(j * 4) + i];

                int fila_mosaico = tile_id / COLS_MOSAICO;
                int col_mosaico = tile_id % COLS_MOSAICO;

                if (fila_mosaico >= FILAS_MOSAICO)
                    throw std::runtime_error("Número de mosaico inválido");
                
                int y_mosaico = fila_mosaico * ALTO_MOSAICO;
                int x_mosaico = col_mosaico * ANCHO_MOSAICO;
                Rectangulo seccion(x_mosaico, y_mosaico, ANCHO_MOSAICO,
                    ALTO_MOSAICO);
                mosaico.renderizar(
                    i * ANCHO_MOSAICO, j * ALTO_MOSAICO, 
                    seccion, textura_celda);
            }
        }
    }

    textura_celda.renderizar(x, y, destino);
}

} // namespace cliente
