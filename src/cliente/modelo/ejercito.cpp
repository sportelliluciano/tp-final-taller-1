#include "cliente/modelo/ejercito.h"

#include "libs/json.hpp"

#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"

#define MIN_TIEMPO_ENTRENAMIENTO 1

#define COLOR_ROJO 1

namespace cliente {

Ejercito::Ejercito(const nlohmann::json& tropas_json, Infraestructura& inf, 
    Terreno& terreno_juego, int id_jugador_actual_, const std::string& casa_) 
: casa(casa_),
  infraestructura(inf),
  terreno(terreno_juego),
  id_jugador_actual(id_jugador_actual_)
{
    using nlohmann::json;

    auto it = tropas_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != tropas_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        tropas_base.emplace(elem.at("id"), Tropa(elem));
        tropas_base_ordenadas.push_back(&tropas_base.at(elem.at("id")));
    }
}

void Ejercito::renderizar(Ventana& ventana, Camara& camara) {
    for (Tropa* tropa : terreno.obtener_tropas_en(camara.obtener_vista())) {
#ifdef DEPURACION_DIBUJO
        /*** Pintar celda ***/
        int celda_x, celda_y;
        terreno.obtener_celda(terreno.obtener_posicion(tropa), 
            celda_x, celda_y);
        
        Posicion grilla = camara.traducir_a_visual(
            terreno.obtener_posicion(celda_x, celda_y));
        Sprite(1).renderizar(ventana, grilla.x, grilla.y);
        /*** Fin pintar celda ***/
#endif
        Posicion visual = camara.traducir_a_visual(terreno.obtener_posicion(tropa));
        tropa->renderizar(ventana, visual.x, visual.y);

        if (tropa->obtener_propietario() != id_jugador_actual) {
            // Agregar marca para identificar el jugador
            ventana.dibujar_rectangulo(visual.x, visual.y, visual.x+1, 
                visual.y+1, COLOR_ROJO);
            ventana.dibujar_rectangulo(visual.x-1, visual.y-1, visual.x+2, 
                visual.y+2, COLOR_ROJO);
            ventana.dibujar_rectangulo(visual.x-2, visual.y, visual.x+3, 
                visual.y+1, COLOR_ROJO);
            ventana.dibujar_rectangulo(visual.x-3, visual.y-1, visual.x+4, 
                visual.y+2, COLOR_ROJO);
        }
    }
}

void Ejercito::actualizar(int t_ms) {
    for (auto& par: tropas) {
        Tropa& tropa = par.second;
        if (!tropa.esta_moviendo())
            continue;
        int x_ant = tropa.obtener_x(), y_ant = tropa.obtener_y();
        tropa.actualizar(t_ms - last_ms);
        terreno.mover_tropa(tropa, x_ant, y_ant);
    }

    int dt_ee = (int)((t_ms - last_ms) * velocidad_ee);
    
    for (auto it = entrenamiento_actual.begin(); 
        it != entrenamiento_actual.end(); ++it) 
    {
        if (it->second <= MIN_TIEMPO_ENTRENAMIENTO)
            continue;
        if (it->second - dt_ee < MIN_TIEMPO_ENTRENAMIENTO)
            it->second = MIN_TIEMPO_ENTRENAMIENTO;
        else
            it->second -= dt_ee;
    }
    
    last_ms = t_ms;
}

void Ejercito::set_tropa_disparando(int id_tropa, bool disparando) {
    tropas.at(id_tropa).set_esta_disparando(disparando);
}

const Tropa& Ejercito::obtener_tropa_base(const std::string& clase) const {
    return tropas_base.at(clase);
}

int Ejercito::obtener_sprite_clase(const std::string& clase) const {
    return tropas_base.at(clase).obtener_sprite_boton();
}

bool Ejercito::esta_habilitada(const std::string& clase) const {
    // for (const std::string& requerimiento : 
    //     tropas_base.at(clase).obtener_requerimientos()) 
    // {
    //     if (!infraestructura.jugador_actual_tiene(requerimiento))
    //         return false;
    // }
    return true;
}

bool Ejercito::esta_entrenando(const std::string& clase) const {
    return obtener_cola_entrenamiento(clase) != 0
        || obtener_segundos_restantes(clase) != 0;
}

int Ejercito::obtener_cola_entrenamiento(const std::string& clase) const {
    auto it = colas_entrenamiento.find(clase);
    if (it == colas_entrenamiento.end())
        return 0;
    return it->second;
}

int Ejercito::obtener_segundos_restantes(const std::string& clase) const {
    auto it = entrenamiento_actual.find(clase);
    if (it == entrenamiento_actual.end())
        return 0;
    return it->second;
}

std::vector<const Tropa*> Ejercito::obtener_tropas_base() const {
    return tropas_base_ordenadas;
}

bool Ejercito::hay_tropas_enemigas_en(const Posicion& punto) {
    for (Tropa *t : terreno.obtener_tropas_en(punto)) {
        if (t->obtener_propietario() != id_jugador_actual)
            return true;
    }
    return false;
}

Tropa* Ejercito::obtener_tropa_enemiga_en(const Posicion& punto) {
    for (Tropa *t : terreno.obtener_tropas_en(punto)) {
        if (t->obtener_propietario() != id_jugador_actual)
            return t;
    }
    return nullptr;
}

std::unordered_set<Tropa*> Ejercito::obtener_tropas_propias_en(
    const Rectangulo& area) 
{
    std::unordered_set<Tropa*> tropas_propias;
    for (Tropa* tropa : terreno.obtener_tropas_en(area)) {
        if (tropa->obtener_propietario() == id_jugador_actual)
            tropas_propias.insert(tropa);
    }

    return tropas_propias;
}

bool Ejercito::existe(int id_tropa) const {
    return tropas.count(id_tropa) != 0;
}

Tropa& Ejercito::obtener(int id_tropa) {
    return tropas.at(id_tropa);
}

void Ejercito::entrenar(const std::string& clase, int tiempo_ms) {
    entrenamiento_actual[clase] = tiempo_ms;
}

void Ejercito::sincronizar_entrenamiento(const std::string& clase, 
    int tiempo_ms) 
{
    entrenamiento_actual[clase] = tiempo_ms;
}

void Ejercito::actualizar_cola_entrenamiento(const std::string& clase, 
    int cantidad) 
{
    colas_entrenamiento[clase] = cantidad;
}

void Ejercito::crear_tropa(int id, const std::string& clase, 
    const std::vector<int>& posicion, int id_jugador, int vida) 
{
    Tropa nueva_tropa = tropas_base.at(clase);
    tropas.emplace(id, nueva_tropa);
    tropas.at(id).inicializar(
        id, 
        Posicion(posicion.at(0), posicion.at(1)),
        vida,
        id_jugador
    );
    terreno.agregar_tropa(tropas.at(id));
}

void Ejercito::mover_tropa(int id, const std::vector<int>& camino) {
    std::vector<std::pair<int, int>> pasos;
    
    for (size_t i=0; i<camino.size(); i+=2) {
        pasos.push_back({camino[i], camino[i+1]});
    }
    int x_ant = tropas.at(id).obtener_x(), 
        y_ant = tropas.at(id).obtener_y();
    tropas.at(id).seguir_camino(pasos);
    terreno.mover_tropa(tropas.at(id), x_ant, y_ant);
}

void Ejercito::sincronizar_tropa(int id, const std::vector<int>& posicion) {
    int x_ant = tropas.at(id).obtener_x(), 
        y_ant = tropas.at(id).obtener_y();
    tropas.at(id).sync_camino(posicion.at(0), posicion.at(1));
    terreno.mover_tropa(tropas.at(id), x_ant, y_ant);
}

void Ejercito::atacar_tropa(int id, int nueva_vida) {
    tropas.at(id).set_vida(nueva_vida);
}

void Ejercito::destruir_tropa(int id) {
    terreno.eliminar_tropa(tropas.at(id));
    tropas.erase(id);
}

} // namespace cliente
