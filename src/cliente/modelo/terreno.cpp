#include "cliente/modelo/terreno.h"

#include <functional>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cliente/modelo/celda.h"
#include "cliente/video/camara.h"
#include "cliente/video/color.h"
#include "cliente/video/ventana.h"
#include "comun/log.h"

#define ANCHO_CELDA 32
#define ALTO_CELDA 32

#define ANCHO_SELECCION 16
#define ALTO_SELECCION 16

namespace cliente {

Terreno::Terreno(const nlohmann::json& mapa) {
    alto = ancho = 0;
    const std::vector<std::vector<int>>& tipos = 
        mapa.at("tipo").get<std::vector<std::vector<int>>>();
    
    //const std::vector<std::vector<int>>& sprites = 
    //    mapa.at("sprite").get<std::vector<std::vector<int>>>();

    //  para mapa generado por el editor
    /*

    const std::vector<std::vector<std::string>>& tipos = 
        mapa.at("tipo").get<std::vector<std::vector<std::string>>>();

     * 
     *  Parseo terrenos.json -> tiene los vectores de 16 ints
     * 
    
    std::map<std::string, DataTerreno> terrenos_posibles;

    std::ifstream entrada(RUTA_TERRENOS_JSON);

    nlohmann::json terrenos_json;

    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for (; it != terrenos_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        nlohmann::json elem = valores_por_defecto;
        elem.update(*it);

        auto it_sprites = elem["sprites"].begin();
        for (int i = 0; it_sprites != elem["sprites"].end(); ++it_sprites) {
            json tile = *it;

            DataTerreno data_terreno;
            data_terreno.tipo = elem["tipo"];
            data_terreno.pos_tiles = tile["sprites"][i]["pos_tiles"];
            
            std::string id = tile["sprites"][i]["id"];
            terrenos_posibles.emplace(id, data_terreno);
        }
    }

     */

    alto = tipos.size();

    if (alto == 0) {
        throw std::runtime_error("Terreno inválido");
    }

    ancho = tipos.at(0).size();

    for (int y=0;y<alto;y++) {
        std::vector<Celda> fila_actual;
        for (int x=0;x<ancho;x++) {
            fila_actual.push_back(
                // aca habria que leer el string que esta en el json del mapa

                // std::string id = tipos[y][x];

                // obtener el DataTerreno del std::map 

                // DataTerreno terreno = terrenos_posibles.find(id)->second;

                // data terreno tiene el vector de pos_tiles y el tipo (int).
                Celda((tipo_celda_t)tipos[y][x]) //, sprites, x, y)
            );
        }
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
    
    obtener_celda(vista.esquina_sup_izq(), celda_inicial_x, celda_inicial_y);
    obtener_celda(vista.esquina_inf_der(), celda_final_x, celda_final_y);

    // Evitar rectángulos negros en los alrededores
    celda_inicial_x -= 2;
    celda_inicial_y -= 2;
    celda_final_x += 2;
    celda_final_y += 2;

    if ((celda_inicial_x == ultimo_celda_x0) && 
       (celda_final_x == ultimo_celda_x1)  && 
       (celda_inicial_y == ultimo_celda_y0) && 
       (celda_final_y == ultimo_celda_y1))
    {
        Posicion visual = camara.traducir_a_visual(
            obtener_posicion(celda_inicial_x, celda_inicial_y));
        ventana
            .obtener_administrador_texturas()
            .obtener_textura("terreno")
            .renderizar(visual.x, visual.y);
        return;
    }
    
    AdministradorTexturas& admin_texturas = 
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura("terreno"))
        admin_texturas.eliminar_textura("terreno"); 

    Textura& textura = admin_texturas.crear_textura("terreno", 
        ventana.ancho() + 128, ventana.alto() + 128);

    textura.limpiar(COLOR_NEGRO);

    for (int x = celda_inicial_x; x <= celda_final_x; x++) {
        if ((x < 0) || (x >= ancho))
            continue;
        for (int y = celda_inicial_y; y <= celda_final_y; y++) {
            if ((y < 0) || (y >= alto))
                continue;
            
            int x_px = (x - celda_inicial_x) * ANCHO_CELDA;
            int y_px = (y - celda_inicial_y) * ALTO_CELDA;
            terreno[y][x].renderizar(ventana, x_px, y_px, textura);
        }
    }

    ultimo_celda_x0 = celda_inicial_x;
    ultimo_celda_x1 = celda_final_x;
    ultimo_celda_y0 = celda_inicial_y;
    ultimo_celda_y1 = celda_final_y;

    Posicion visual = camara.traducir_a_visual(
        obtener_posicion(celda_inicial_x, celda_inicial_y));
    textura.renderizar(visual.x, visual.y);
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
    return tropa->obtener_posicion();
}

Posicion Terreno::obtener_centro(const Edificio* edificio) {
    Posicion esquina_superior = obtener_posicion(edificio);
    Posicion esquina_inferior = obtener_posicion(
        edificio->obtener_celda_x() + edificio->obtener_ancho_celdas(),
        edificio->obtener_celda_y() + edificio->obtener_alto_celdas()
    );

    int x_medio = (esquina_superior.x + esquina_inferior.x) / 2;
    int y_medio = (esquina_superior.y + esquina_inferior.y) / 2;
    return Posicion(x_medio, y_medio);
}

void Terreno::para_cada_celda_en(const Rectangulo& area, 
    std::function<bool(int, int, bool)> accion) 
{
    int celda_inicial_x, celda_inicial_y,
        celda_final_x, celda_final_y;
    
    obtener_celda(area.esquina_sup_izq(), celda_inicial_x, celda_inicial_y);
    obtener_celda(area.esquina_inf_der(), celda_final_x, celda_final_y);

    celda_final_x += 1;
    celda_final_y += 1;

    for (int x = celda_inicial_x; x <= celda_final_x; x++) {
        if ((x < 0) || (x >= ancho))
            continue;
        for (int y = celda_inicial_y; y <= celda_final_y; y++) {
            if ((y < 0) || (y >= alto))
                continue;
            
            Posicion esq_sup_izq_celda = obtener_posicion(x, y);
            Posicion esq_inf_der_celda = obtener_posicion(x + 1, y + 1);

            bool contenida_totalmente = 
                (area.contiene_punto(esq_sup_izq_celda)
                    && area.contiene_punto(esq_inf_der_celda));


            if (!accion(x, y, contenida_totalmente))
                return;
        }
    }
}

std::unordered_set<Edificio*> Terreno::obtener_edificios_en(
    const Rectangulo& area) 
{
    std::unordered_set<Edificio*> edificios;
    
    para_cada_celda_en(area, 
        [this, &edificios] (int x, int y, bool) {
            if (terreno[y][x].contiene_edificio())
                edificios.insert(&terreno[y][x].obtener_edificio());
            return true;
        });
    
    return edificios;
}

std::unordered_set<Tropa*> Terreno::obtener_tropas_en(const Rectangulo& area) {
    std::unordered_set<Tropa*> tropas;

    para_cada_celda_en(area, 
        [this, &tropas, &area] (int x, int y, bool contenida_totalmente) {
            if (!terreno[y][x].contiene_tropas()) {
                return true;
            }
            if (contenida_totalmente) {
                for (Tropa* tropa : terreno[y][x].obtener_tropas()) {
                    tropas.insert(tropa);
                }
            } else {
                for (Tropa* tropa : terreno[y][x].obtener_tropas()) {
                    Posicion pos_tropa = tropa->obtener_posicion();
                    if (area.contiene_punto(pos_tropa))
                        tropas.insert(tropa);
                }
            }
            return true;
        });
    
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

std::unordered_set<Tropa*> Terreno::obtener_tropas_en(const Posicion& punto) {
    Rectangulo area = Rectangulo(
        punto.x - (ANCHO_SELECCION / 2), punto.y - (ALTO_SELECCION / 2),
        ANCHO_SELECCION, ALTO_SELECCION);

    return obtener_tropas_en(area);
}

Edificio* Terreno::obtener_edificio_en(const Posicion& pos) {
    int celda_x = pos.x / ANCHO_CELDA;
    int celda_y = pos.y / ALTO_CELDA;

    if ((celda_x < 0) || (celda_x >= ancho))
        return nullptr;
    if ((celda_y < 0) || (celda_y >= alto))
        return nullptr;
    
    if (terreno[celda_y][celda_x].contiene_edificio())
        return &terreno[celda_y][celda_x].obtener_edificio();
    
    return nullptr;
}

void Terreno::agregar_tropa(Tropa& tropa) {
    int celda_x = tropa.obtener_posicion().x / ANCHO_CELDA;
    int celda_y = tropa.obtener_posicion().y / ALTO_CELDA;

    if ((celda_x >= ancho) || (celda_y >= alto) || 
        (celda_x < 0) || (celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].agregar_tropa(tropa);
}

void Terreno::mover_tropa(Tropa& tropa, const Posicion& anterior) {
    int nuevo_celda_x = tropa.obtener_posicion().x / ANCHO_CELDA;
    int nuevo_celda_y = tropa.obtener_posicion().y / ALTO_CELDA;

    int celda_x = anterior.x / ANCHO_CELDA;
    int celda_y = anterior.y / ALTO_CELDA;

    if ((nuevo_celda_x == celda_x) && (nuevo_celda_y == celda_y))
        return;

    if ((celda_x >= ancho) || (celda_y >= alto) || 
        (celda_x < 0) || (celda_y < 0) ||
        (nuevo_celda_x >= ancho) || (nuevo_celda_y >= alto) || 
        (nuevo_celda_x < 0) || (nuevo_celda_y < 0))
    {
        throw std::runtime_error("Terreno::agregar_tropa: Fuera de rango");
    }

    terreno[celda_y][celda_x].eliminar_tropa(tropa);
    terreno[nuevo_celda_y][nuevo_celda_x].agregar_tropa(tropa);
}

void Terreno::eliminar_tropa(const Tropa& tropa) {
    int celda_x = tropa.obtener_posicion().x / ANCHO_CELDA;
    int celda_y = tropa.obtener_posicion().y / ALTO_CELDA;

    if ((celda_x >= ancho) || (celda_y >= alto) || 
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
            if (((celda_x + i) >= ancho) || ((celda_y + j) >= alto)) {
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
            if (((celda_x + i) >= ancho) || ((celda_y + j) >= alto)) {
                throw std::runtime_error(
                    "Terreno::agregar_edificio: Fuera de rango");
            }

            terreno[celda_y + j][celda_x + i].eliminar_edificio(edificio);
        }
    }
}

int Terreno::obtener_ancho_px() const {
    return ancho * ANCHO_CELDA;
}

int Terreno::obtener_alto_px() const {
    return alto * ALTO_CELDA;
}

Terreno::~Terreno() {

}

} // namespace cliente
