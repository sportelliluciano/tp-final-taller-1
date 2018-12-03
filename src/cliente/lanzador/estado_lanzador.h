#ifndef _ESTADO_LANZADOR_H_
#define _ESTADO_LANZADOR_H_

#include "cliente/red/partida.h"
#include "cliente/red/servidor.h"
#include "cliente/lanzador/ui_lanzador.fwd.h"

class EstadoLanzador {
public:
    EstadoLanzador(Ui::Lanzador *ui_, cliente::Partida& partida, 
        cliente::Servidor*& servidor);
    virtual void entrar() = 0;
    virtual EstadoLanzador* btn_iniciar_juego_click();
    virtual EstadoLanzador* btn_conectar_click();
    virtual EstadoLanzador* btn_actualizar_salas_click();
    virtual EstadoLanzador* btn_elegir_salas_click();
    virtual EstadoLanzador* btn_crear_sala_click();
    virtual EstadoLanzador* list_salas_item_clicked();
    virtual void salir() = 0;
    virtual ~EstadoLanzador() { }

protected:
    Ui::Lanzador *ui;
    cliente::Partida& partida;
    cliente::Servidor*& servidor;

};

#endif // _ESTADO_LANZADOR_H_
