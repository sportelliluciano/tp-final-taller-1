#include "cliente/lanzador/estado_lanzador.h"

EstadoLanzador::EstadoLanzador(Ui::Lanzador *ui_, cliente::Partida& partida, 
    cliente::Servidor*& servidor) 
: ui(ui_), partida(partida), servidor(servidor) { }

EstadoLanzador* EstadoLanzador::btn_iniciar_juego_click() {
    return this;
}

EstadoLanzador* EstadoLanzador::btn_conectar_click() {
    return this;
}

EstadoLanzador* EstadoLanzador::btn_actualizar_salas_click() {
    return this;
}

EstadoLanzador* EstadoLanzador::btn_elegir_salas_click() {
    return this;
}

EstadoLanzador* EstadoLanzador::btn_crear_sala_click() {
    return this;
}

EstadoLanzador* EstadoLanzador::list_salas_item_clicked() {
    return this;
}
