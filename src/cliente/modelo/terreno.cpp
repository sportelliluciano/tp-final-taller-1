#include "cliente/modelo/terreno.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cliente/video/ventana.h"

#define ANCHO_CELDA 32
#define ALTO_CELDA 32

#define ANCHO_MOSAICO 8
#define ALTO_MOSAICO 8

#define COLS_MOSAICO 20
#define FILAS_MOSAICO 40

#define RUTA_MOSAICO "./assets/terrain/d2k_BLOXBASE.bmp"

namespace cliente {

Terreno::Terreno(const char *ruta_csv) {
    std::ifstream entrada(ruta_csv);
    if (!entrada.good())
        throw std::runtime_error("No se pudo abrir el archivo de terreno");
    
    int x = 0, y = 0;
    alto_ = ancho_ = 0;

    while (!entrada.eof()) {
        std::string linea, tile;
        std::getline(entrada, linea);
        std::stringstream s_linea(linea);
        
        std::vector<Celda> fila_actual;
        
        while (std::getline(s_linea, tile, ';')) {
            std::stringstream s_tile(tile);
            int tile_no = 0;
            if (!(s_tile >> tile_no))
                throw std::runtime_error("Archivo de terreno inválido");
            
            fila_actual.push_back(Celda(tile_no, x, y));
            
            if (x > ancho_)
                ancho_ = x;
            
            x++;
        }
        
        if (y > alto_)
            alto_ = y;

        x = 0;
        y++;
        terreno.push_back(fila_actual);
    }

    set_camara(0, 0);
}

const Celda& Terreno::at(int x, int y) const {
    return terreno[y][x];
}

int Terreno::alto() const {
    return alto_;
}

int Terreno::ancho() const {
    return ancho_;
}

void Terreno::set_camara(int camara_x_, int camara_y_) {
    camara_x = camara_x_;
    camara_y = camara_y_;
    is_dirty = true;
}

void Terreno::renderizar_mosaico(Ventana& ventana, int x, int y, 
    Textura& destino) {
    const Textura& mosaico = ventana
        .obtener_administrador_texturas()
        .cargar_imagen(RUTA_MOSAICO);
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

    //int tile_id = ids_arena[arena];
    //arena = (arena + 1) % ids_arena.size();
    static int arena = 0;
    static int roca = 0;

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            int tile_id = 0;
            if (at(x, y).numero_mosaico() == 0) {
                tile_id = ids_arena[arena];
                arena = (arena + 1) % ids_arena.size();
            } else if (at(x, y).numero_mosaico() == 1) {
                tile_id = ids_roca[roca];
                roca = (roca + 1) % ids_roca.size();
            } else {
                tile_id = at(x, y).numero_mosaico();
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
                ((x - camara_x) * 4 + i) * ANCHO_MOSAICO, 
                ((y - camara_y) * 4 + j) * ALTO_MOSAICO, 
                seccion, destino);
        }
    }
}

void Terreno::renderizar(Ventana& ventana) {
    if (!is_dirty) {
        ventana
            .obtener_administrador_texturas()
            .obtener_textura("terreno")
            .renderizar(0, 0);
        ventana.dibujar_grilla();
        return;
    }
    
    Textura& textura = ventana
        .obtener_administrador_texturas()
        .crear_textura("terreno", ventana.ancho(), ventana.alto());
    
    /**
     * Se renderizan siempre tantas celdas como entren en la ventana.
     * Las celdas que no estén en el tablero serán simplemente mosaicos en
     * negro.
     */
    int ancho_ventana = ventana.ancho() / ANCHO_MOSAICO;
    int alto_ventana = ventana.alto() / ALTO_MOSAICO;

    for (int x = camara_x; x < camara_x + ancho_ventana; x++) {
        if ((x < 0) || (x > ancho()))
            continue;
        for (int y = camara_y; y < camara_y + alto_ventana; y++) {
            if ((y < 0) || (y > alto()))
                continue;

            renderizar_mosaico(ventana, x, y, textura);
        }
    }

    is_dirty = false;
    textura.renderizar(0, 0);
    ventana.dibujar_grilla();
}

void Terreno::convertir_a_px(int x, int y, int& x_px, int& y_px) const {
    x_px = (x - camara_x) * ANCHO_CELDA;
    y_px = (y - camara_y) * ALTO_CELDA;
}

bool Terreno::esta_en_camara(std::vector<Celda> celdas, Ventana& ventana) {
    int ancho_ventana = ventana.ancho() / ANCHO_MOSAICO;
    int alto_ventana = ventana.alto() / ALTO_MOSAICO;
    int limite_x = camara_x + ancho_ventana;
    int limite_y = camara_y + alto_ventana;

    for (auto it = celdas.begin(); it != celdas.end(); ++it) {
        if ((camara_x <= it->x()) && (it->x() < limite_x)) {
            if ((camara_y <= it->y()) && (it->y() < limite_y))
                return true;
        }
    }

    return false;
}

std::vector<Celda> Terreno::obtener_celdas_visibles(Ventana& ventana) {
    /**
     * Se renderizan siempre tantas celdas como entren en la ventana.
     * Las celdas que no estén en el tablero serán simplemente mosaicos en
     * negro.
     */
    int ancho_ventana = ventana.ancho() / ANCHO_MOSAICO;
    int alto_ventana = ventana.alto() / ALTO_MOSAICO;

    std::vector<Celda> resultado;

    for (int x = camara_x; x < camara_x + ancho_ventana; x++) {
        if ((x < 0) || (x > ancho()))
            continue;
        for (int y = camara_y; y < camara_y + alto_ventana; y++) {
            if ((y < 0) || (y > alto()))
                continue;

            resultado.push_back(terreno[y][x]);
        }
    }

    return resultado;
}

Terreno::~Terreno() {

}

} // namespace cliente
