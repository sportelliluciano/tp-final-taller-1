#ifndef _ESTADO_CONECTADO_H_
#define _ESTADO_CONECTADO_H_

#include "cliente/lanzador/estado_lanzador.h"

class EstadoConectado : public EstadoLanzador {
public:
    EstadoConectado(Ui::Lanzador *ui_, cliente::Partida& partida_, 
        cliente::Servidor*& servidor_);
    virtual void entrar();
    EstadoLanzador* btn_iniciar_juego_click() override;
    EstadoLanzador* btn_conectar_click() override;
    EstadoLanzador* btn_actualizar_salas_click() override;
    EstadoLanzador* btn_crear_sala_click() override;
    EstadoLanzador* btn_elegir_salas_click() override;
    virtual void salir();
    virtual ~EstadoConectado() { }


private:
    void actualizar_estado_ui(bool habilitar);
};

#endif // _ESTADO_CONECTADO_H_
