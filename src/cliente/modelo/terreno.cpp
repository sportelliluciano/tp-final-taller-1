#include "cliente/modelo/terreno.h"

#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cliente/modelo/celda.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"
#include "cliente/video/log.h"

#define ANCHO_CELDA 32
#define ALTO_CELDA 32

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

    ultimo_celda_x0 = ancho + 1;
    ultimo_celda_x1 = ancho + 1;
    ultimo_celda_y0 = alto + 1;
    ultimo_celda_y1 = alto + 1;
}

void Terreno::renderizar(Ventana& ventana, Camara& camara) {
    Rectangulo vista = camara.obtener_vista();
    
    int celda_inicial_x, celda_inicial_y,
        celda_final_x, celda_final_y;
    
    obtener_celda(vista.esq_sup_izq, celda_inicial_x, celda_inicial_y);
    obtener_celda(vista.esq_inf_der, celda_final_x, celda_final_y);

    // Evitar rectángulo negro al final
    celda_final_x += 1;
    celda_final_y += 1;

    if ((celda_inicial_x == ultimo_celda_x0) && 
       (celda_final_x == ultimo_celda_x1)  && 
       (celda_inicial_y == ultimo_celda_y0) && 
       (celda_final_y == ultimo_celda_y1))
    {
        Posicion visual = camara.traducir_a_visual(vista.esq_sup_izq);
        ventana
            .obtener_administrador_texturas()
            .obtener_textura("terreno")
            .renderizar(visual.x, visual.y);
        ventana.dibujar_grilla(visual.x, visual.y);
        return;
    }
    
    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura("terreno"))
        admin_texturas.eliminar_textura("terreno"); 

    Textura& textura = admin_texturas.crear_textura("terreno", 
        ventana.ancho(), ventana.alto());

    textura.limpiar(0, 0, 0, 255);

    for (int x = celda_inicial_x; x <= celda_final_x; x++) {
        if ((x < 0) || (x > ancho))
            continue;
        for (int y = celda_inicial_y; y <= celda_final_y; y++) {
            if ((y < 0) || (y > alto))
                continue;
            
            Posicion visual = camara.traducir_a_visual(obtener_posicion(x, y));
            terreno[y][x].renderizar(ventana, visual.x, visual.y, textura);
        }
    }

    ultimo_celda_x0 = celda_inicial_x;
    ultimo_celda_x1 = celda_final_x;
    ultimo_celda_y0 = celda_inicial_y;
    ultimo_celda_y1 = celda_final_y;

    Posicion visual = camara.traducir_a_visual(vista.esq_sup_izq);
    textura.renderizar(visual.x, visual.y);
    ventana.dibujar_grilla(visual.x, visual.y);
}

void Terreno::obtener_celda(const Posicion& pos, int& celda_x, int& celda_y) {
    celda_x = pos.x / ANCHO_CELDA;
    celda_y = pos.y / ALTO_CELDA;
}

Posicion Terreno::obtener_posicion(int celda_x, int celda_y) {
    return Posicion(celda_x * ANCHO_CELDA, celda_y * ALTO_CELDA);
}

Posicion Terreno::obtener_posicion(const Edificio* edificio) {
    return obtener_posicion(edificio->obtener_celda_x(), 
        edificio->obtener_celda_y());
}

Posicion Terreno::obtener_posicion(const Tropa* tropa) {
    return Posicion(tropa->obtener_x(), tropa->obtener_y());
}

void Terreno::para_cada_celda_en(const Rectangulo& area, 
    std::function<bool(int, int)> accion, bool considerar_overflow) 
{
    int celda_inicial_x, celda_inicial_y,
        celda_final_x, celda_final_y;
    
    obtener_celda(area.esq_sup_izq, celda_inicial_x, celda_inicial_y);
    obtener_celda(area.esq_inf_der, celda_final_x, celda_final_y);

    if (considerar_overflow) {
        celda_final_x += 1;
        celda_final_y += 1;
    }

    for (int x = celda_inicial_x; x <= celda_final_x; x++) {
        if ((x < 0) || (x > ancho))
            continue;
        for (int y = celda_inicial_y; y <= celda_final_y; y++) {
            if ((y < 0) || (y > alto))
                continue;
            if (!accion(x, y))
                return;
        }
    }
}

std::unordered_set<Edificio*> Terreno::obtener_edificios_en(
    const Rectangulo& area) 
{
    std::unordered_set<Edificio*> edificios;
    
    para_cada_celda_en(area, 
        [this, &edificios] (int x, int y) {
            if (terreno[y][x].contiene_edificio())
                edificios.insert(&terreno[y][x].obtener_edificio());
            return true;
        }, false);
    
    return edificios;
}

std::unordered_set<Tropa*> Terreno::obtener_tropas_en(const Rectangulo& area) {
    std::unordered_set<Tropa*> tropas;
    
    para_cada_celda_en(area, 
        [this, &tropas] (int x, int y) {
            if (terreno[y][x].contiene_tropas()) {
                for (Tropa* tropa : terreno[y][x].obtener_tropas()) {
                    tropas.insert(tropa);
                }
            }
            return true;
        }, false);
    
    return tropas;
}

bool Terreno::es_construible(int x_celda, int y_celda) const {
    if ((x_celda < 0) || (x_celda >= ancho) || 
        (y_celda < 0) || (y_celda >= alto)) 
    {
        return false;
    }
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

Tropa* Terreno::obtener_tropa_en(const Posicion& pos) {
    Tropa* tropa = nullptr;
    
    para_cada_celda_en(Rectangulo(pos.x, pos.y, ANCHO_CELDA, ALTO_CELDA),
        [this, &tropa] (int x, int y) {
            if (terreno[y][x].contiene_tropas()) {
                tropa = terreno[y][x].obtener_tropas().front();
                return false;
            }
            return true;
        }, true);

    return tropa;
}

Edificio* Terreno::obtener_edificio_en(const Posicion& pos) {
    int celda_x = pos.x / ANCHO_CELDA;
    int celda_y = pos.y / ALTO_CELDA;

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
        (nuevo_celda_x > ancho) || (nuevo_celda_y > alto) || 
        (nuevo_celda_x < 0) || (nuevo_celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].eliminar_tropa(tropa);
    terreno[nuevo_celda_y][nuevo_celda_x].agregar_tropa(tropa);
    log_advertencia("Cambio de celdas: (%d,%d) => (%d,%d)", celda_x, celda_y,
        nuevo_celda_x, nuevo_celda_y);
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
