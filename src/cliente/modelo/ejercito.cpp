#include "cliente/modelo/ejercito.h"

#include <fstream>

#include "libs/json.hpp"

#include "cliente/modelo/terreno.h"

#define MIN_TIEMPO_ENTRENAMIENTO 1

namespace cliente {

Ejercito::Ejercito(int id_jugador_actual_, Terreno& terreno_juego, 
    const nlohmann::json& tropas_json) 
: terreno(terreno_juego),
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
    }
}

void Ejercito::renderizar(Ventana& ventana, Camara& camara) {
    for (Tropa* tropa : terreno.obtener_tropas_en(camara.obtener_vista())) {
        /*** Pintar celda ***/
        
        int celda_x, celda_y;
        terreno.obtener_celda(terreno.obtener_posicion(tropa), 
            celda_x, celda_y);
        
        Posicion visual = camara.traducir_a_visual(
            terreno.obtener_posicion(celda_x, celda_y));
        Sprite(1).renderizar(ventana, visual.x, visual.y);
        /*** Fin pintar celda ***/

        visual = camara.traducir_a_visual(terreno.obtener_posicion(tropa));
        tropa->renderizar(ventana, visual.x, visual.y);
        ventana.dibujar_rectangulo(visual.x, visual.y, visual.x+1, visual.y+1, 3);
        ventana.dibujar_rectangulo(visual.x-1, visual.y-1, visual.x+2, visual.y+2, 3);
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

int Ejercito::obtener_sprite_clase(const std::string& clase) const {
    return tropas_base.at(clase).obtener_sprite_boton();
}

bool Ejercito::esta_habilitada(const std::string& clase) const {
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
    std::vector<const Tropa*> resultado;
    for (auto& it : tropas_base) {
        resultado.push_back(&it.second);
    }
    return resultado;
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
    const std::vector<int>& posicion, int id_jugador) 
{
    Tropa nueva_tropa = tropas_base.at(clase);
    tropas.emplace(id, nueva_tropa);
    tropas.at(id).inicializar(
        id, 
        Posicion(posicion.at(0), posicion.at(1)),
        100,
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
