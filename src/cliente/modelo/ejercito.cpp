#include "cliente/modelo/ejercito.h"

#include "cliente/modelo/terreno.h"

namespace cliente {

Ejercito::Ejercito(Terreno& terreno_juego) : terreno(terreno_juego) { 
    crear(0, "", 300, 300);
    tropas.at(0).seguir_camino(
        {{300, 300}, {300, 400}, {300, 500}}
    );
    // {"id":4, "id_tropa":0,"camino":[300, 300, 300, 340, 300, 360, 300, 380, 300, 400]}
}

void Ejercito::renderizar(Ventana& ventana) {
    for (const Tropa* tid : terreno.obtener_tropas_visibles(ventana)) {
        Tropa& tropa = tropas.at(tid->obtener_id());
        int x, y;

        /*** Pintar celda ***/
        int x_celda = tropa.obtener_x() / 32,
            y_celda = tropa.obtener_y() / 32;

        terreno.convertir_a_px(x_celda, y_celda, x, y);
        Sprite(1).renderizar(ventana, x, y);
        /*** Fin pintar celda ***/

        terreno.obtener_posicion_visual(tropa, x, y);

        if (unidades_seleccionadas.find(tid->obtener_id()) 
            != unidades_seleccionadas.end()) 
        {
            ventana
                .obtener_administrador_texturas()
                .cargar_imagen("./assets/nuevos/unidad-seleccionada.png")
                .renderizar(x, y);
        }
        
        
        tropa.renderizar(ventana, x, y);
    }
}

void Ejercito::actualizar(int t_ms) {
    for (auto& par: tropas) {
        Tropa& tropa = par.second;
        if (!tropa.esta_moviendo())
            continue;
        int x_ant = tropa.obtener_x() , y_ant = tropa.obtener_y();
        tropa.actualizar(t_ms - last_ms);
        terreno.mover_tropa(tropa, x_ant, y_ant);
    }
    last_ms = t_ms;
}

void Ejercito::crear(int id, const std::string& , int x, int y) {
    tropas.emplace(id, Tropa(id, x, y));
    terreno.agregar_tropa(tropas.at(id));
}

void Ejercito::mover(int id, int x_destino, int y_destino) {
    tropas.at(id).caminar_hacia(x_destino, y_destino);
}

void Ejercito::seleccionar(const std::unordered_set<const Tropa*>& unidades) {
    unidades_seleccionadas = std::unordered_set<int>();

    for (const Tropa* tropa : unidades) {
        unidades_seleccionadas.insert(tropa->obtener_id());
    }
}

void Ejercito::sincronizar_tropa(int id_tropa, int x, int y) {
    tropas.at(id_tropa).sync_camino(x, y);
}

void Ejercito::indicar_camino_tropa(int id_tropa, 
    const std::vector<std::pair<int, int>>& camino)
{
    tropas.at(id_tropa).seguir_camino(camino);
}

void Ejercito::destruir(int id) {
    terreno.eliminar_tropa(tropas.at(id));
    tropas.erase(id);
}

} // namespace cliente
