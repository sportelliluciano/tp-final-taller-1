#include "cliente/modelo/terreno.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cliente/video/ventana.h"

#define ANCHO_CELDA 32
#define ALTO_CELDA 32

#define LIM_INF_CAMARA_X -10
#define LIM_SUP_CAMARA_X  10
#define LIM_INF_CAMARA_Y -10
#define LIM_SUP_CAMARA_Y  10

namespace cliente {

Terreno::Terreno(const char *ruta_csv) {
    std::ifstream entrada(ruta_csv);
    if (!entrada.good())
        throw std::runtime_error("No se pudo abrir el archivo de terreno");
    
    int x = 0, y = 0;
    alto = ancho = 0;

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
            
            tipo_celda_t tipo = CELDA_ARENA;
            if (tile_no == 1)
                tipo = CELDA_ROCA;

            fila_actual.push_back(Celda(tipo));
            
            if (x > ancho)
                ancho = x;
            
            x++;
        }
        
        if (y > alto)
            alto = y;

        x = 0;
        y++;
        terreno.push_back(fila_actual);
    }

    set_camara(0, 0);
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
    
    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura("terreno"))
        admin_texturas.eliminar_textura("terreno"); 

    Textura& textura = admin_texturas.crear_textura("terreno", 
        ventana.ancho(), ventana.alto());

    textura.limpiar(0, 0, 0, 255);

    int ancho_ventana = ventana.ancho() / ANCHO_CELDA;
    int alto_ventana = ventana.alto() / ALTO_CELDA;

    for (int x = camara_x; x <= camara_x + ancho_ventana; x++) {
        if ((x < 0) || (x > ancho))
            continue;
        for (int y = camara_y; y <= camara_y + alto_ventana; y++) {
            if ((y < 0) || (y > alto))
                continue;
            
            int x_px, y_px;
            x_px = (x - camara_x) * ANCHO_CELDA;
            y_px = (y - camara_y) * ALTO_CELDA;

            terreno[y][x].renderizar(ventana, x_px, y_px, textura);
        }
    }

    is_dirty = false;
    textura.renderizar(0, 0);
    ventana.dibujar_grilla();
}

void Terreno::set_camara(int nuevo_camara_x, int nuevo_camara_y) {
    if (LIM_INF_CAMARA_X > nuevo_camara_x)
        nuevo_camara_x = LIM_INF_CAMARA_X;
    if (LIM_INF_CAMARA_Y > nuevo_camara_y)
        nuevo_camara_y = LIM_INF_CAMARA_Y;
    
    if (nuevo_camara_x > ancho + LIM_SUP_CAMARA_X)
        nuevo_camara_x = ancho + LIM_SUP_CAMARA_X;
    if (nuevo_camara_y > alto + LIM_SUP_CAMARA_Y)
        nuevo_camara_y = alto + LIM_SUP_CAMARA_Y;

    if ((camara_x == nuevo_camara_x) && (camara_y == nuevo_camara_y))
        return;

    camara_x = nuevo_camara_x;
    camara_y = nuevo_camara_y;
    is_dirty = true;
}

void Terreno::mover_camara(int dx, int dy) {
    if ((dx == 0) && (dy == 0))
        return;
    
    set_camara(camara_x + dx, camara_y + dy);
}

std::unordered_set<const Edificio*> Terreno::obtener_edificios_visibles(
    Ventana& ventana) {
    int ancho_ventana = ventana.ancho() / ANCHO_CELDA;
    int alto_ventana = ventana.alto() / ALTO_CELDA;

    std::unordered_set<const Edificio*> resultado;

    for (int x = camara_x; x <= camara_x + ancho_ventana; x++) {
        if ((x < 0) || (x > ancho))
            continue;
        for (int y = camara_y; y <= camara_y + alto_ventana; y++) {
            if ((y < 0) || (y > alto))
                continue;

            if (terreno[y][x].contiene_edificio())
                resultado.insert(&terreno[y][x].obtener_edificio());
        }
    }

    return resultado;
}

std::unordered_set<const Tropa*> Terreno::obtener_tropas_visibles(
    Ventana& ventana) {
    int ancho_ventana = ventana.ancho() / ANCHO_CELDA;
    int alto_ventana = ventana.alto() / ALTO_CELDA;

    std::unordered_set<const Tropa*> resultado;

    for (int x = camara_x; x <= camara_x + ancho_ventana; x++) {
        if ((x < 0) || (x > ancho))
            continue;
        for (int y = camara_y; y <= camara_y + alto_ventana; y++) {
            if ((y < 0) || (y > alto))
                continue;

            if (terreno[y][x].contiene_tropas()) {
                for (const Tropa* tropa : terreno[y][x].obtener_tropas()) {
                    resultado.insert(tropa);
                }
            }
        }
    }

    return resultado;
}

void Terreno::obtener_posicion_visual(const Tropa& tropa, int& x_px, int& y_px) 
{
    x_px = tropa.obtener_x() - (camara_x * ANCHO_CELDA);
    y_px = tropa.obtener_y() - (camara_y * ALTO_CELDA);

}

void Terreno::obtener_posicion_visual(const Edificio& edificio, int& x_px, 
    int& y_px) 
{
    convertir_a_px(edificio.obtener_celda_x(), edificio.obtener_celda_y(),
        x_px, y_px);
}

void Terreno::convertir_a_px(int x_celda, int y_celda, int& x_px, int& y_px) {
    x_px = (x_celda - camara_x) * ANCHO_CELDA;
    y_px = (y_celda - camara_y) * ALTO_CELDA;
}

void Terreno::calcular_celda(int x_px, int y_px, int& x_celda, int& y_celda) {
    x_celda = (x_px / ANCHO_CELDA) + camara_x;
    y_celda = (y_px / ALTO_CELDA) + camara_y;
}

bool Terreno::es_construible(int x_celda, int y_celda) const {
    return terreno[y_celda][x_celda].es_construible();
}

/**
 * Intercambia el contenido de a con el contenido de b.
 */
static inline void swap(int& a, int& b) {
    int tmp = b;
    b = a;
    a = tmp;
}

std::unordered_set<Tropa*>
    Terreno::seleccionar_unidades(int x0, int y0, int x1, int y1) {
    if (x0 > x1)
        swap(x0, x1);
    
    if (y0 > y1)
        swap(y0, y1);
    
    int celda_x0 = (x0 / ANCHO_CELDA) + camara_x;
    int celda_x1 = (x1 / ANCHO_CELDA) + camara_x;
    int celda_y0 = (y0 / ALTO_CELDA) + camara_y;
    int celda_y1 = (y1 / ALTO_CELDA) + camara_y;

    std::cout << "Seleccionando desde (" << celda_x0 << "," << celda_y0 << ") "
              << "hasta (" << celda_x1 << "," << celda_y1  << ")" << std::endl;

    std::unordered_set<Tropa*> tropas_seleccionadas;

    for (int x = celda_x0; x < celda_x1; x++) {
        for (int y = celda_y0; y < celda_y1; y++) {
            for (Tropa* t : terreno[y][x].obtener_tropas()) {
                tropas_seleccionadas.insert(t);
            }
        }
    }
    
    return tropas_seleccionadas;
}

Edificio* Terreno::obtener_edificio_en(int x, int y) {
    int celda_x = (x / ANCHO_CELDA) + camara_x;
    int celda_y = (y / ALTO_CELDA) + camara_y;
    if ((celda_x < 0) || (celda_x > ancho))
        return nullptr;
    if ((celda_y < 0) || (celda_y > alto))
        return nullptr;
    
    if (terreno[celda_y][celda_x].contiene_edificio())
        return &terreno[celda_y][celda_x].obtener_edificio();
    
    return nullptr;
}

void Terreno::agregar_tropa(Tropa& tropa) {
    int celda_x = tropa.obtener_x() / ANCHO_CELDA;
    int celda_y = tropa.obtener_y() / ALTO_CELDA;

    if ((celda_x > ancho) || (celda_y > alto) || 
        (celda_x < 0) || (celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].agregar_tropa(tropa);
}

void Terreno::mover_tropa(Tropa& tropa, int x_ant, int y_ant) {
    int nuevo_celda_x = tropa.obtener_x() / ANCHO_CELDA;
    int nuevo_celda_y = tropa.obtener_y() / ALTO_CELDA;

    int celda_x = x_ant / ANCHO_CELDA;
    int celda_y = y_ant / ALTO_CELDA;

    if ((nuevo_celda_x == celda_x) && (nuevo_celda_y == celda_y))
        return;

    if ((celda_x > ancho) || (celda_y > alto) || 
        (celda_x < 0) || (celda_y < 0) ||
        (nuevo_celda_x >ancho) || (nuevo_celda_y > alto) || 
        (nuevo_celda_x < 0) || (nuevo_celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].eliminar_tropa(tropa);
    terreno[nuevo_celda_y][nuevo_celda_x].agregar_tropa(tropa);
}

void Terreno::eliminar_tropa(const Tropa& tropa) {
    int celda_x = tropa.obtener_x() / ANCHO_CELDA;
    int celda_y = tropa.obtener_y() / ALTO_CELDA;

    if ((celda_x > ancho) || (celda_y > alto) || 
        (celda_x < 0) || (celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].eliminar_tropa(tropa);
}

void Terreno::agregar_edificio(Edificio& edificio) {
    int celda_x = edificio.obtener_celda_x();
    int celda_y = edificio.obtener_celda_y();

    if ((celda_x < 0) || (celda_y < 0))
        throw std::runtime_error("Terreno::agregar_edificio: Fuera de rango");
    
    for (int i=0;i<edificio.obtener_ancho_celdas();i++) {
        for (int j=0;j<edificio.obtener_alto_celdas();j++) {        
            if (((celda_x + i) > ancho) || ((celda_y + j) > alto)) {
                throw std::runtime_error(
                    "Terreno::agregar_edificio: Fuera de rango");
            }

            terreno[celda_y + j][celda_x + i].set_edificio(edificio);
        }
    }
}

void Terreno::eliminar_edificio(const Edificio& edificio) {
    int celda_x = edificio.obtener_celda_x();
    int celda_y = edificio.obtener_celda_y();

    if ((celda_x < 0) || (celda_y < 0))
        throw std::runtime_error("Terreno::agregar_edificio: Fuera de rango");
    
    for (int i=0;i<edificio.obtener_ancho_celdas();i++) {
        for (int j=0;j<edificio.obtener_alto_celdas();j++) {        
            if (((celda_x + i) > ancho) || ((celda_y + j) > alto)) {
                throw std::runtime_error(
                    "Terreno::agregar_edificio: Fuera de rango");
            }

            terreno[celda_y + j][celda_x + i].eliminar_edificio(edificio);
        }
    }
}

Terreno::~Terreno() {

}

} // namespace cliente
