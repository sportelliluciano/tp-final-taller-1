#include "cliente/modelo/ejercito.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Ejercito::Ejercito(Terreno& terreno_juego) : terreno(terreno_juego) { }

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
    last_ms = t_ms;
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
    tropas.emplace(id, Tropa(id, posicion.at(0), posicion.at(1)));
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
