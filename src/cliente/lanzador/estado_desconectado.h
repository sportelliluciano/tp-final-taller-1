#ifndef _ESTADO_DESCONECTADO_H_
#define _ESTADO_DESCONECTADO_H_

#include "cliente/lanzador/estado_lanzador.h"

class EstadoDesconectado : public EstadoLanzador {
public:
    EstadoDesconectado(Ui::Lanzador *ui_, cliente::Partida& partida_, 
        cliente::Servidor*& servidor_);
    virtual void entrar();
    EstadoLanzador* btn_iniciar_juego_click() override;
    EstadoLanzador* btn_conectar_click() override;
    virtual void salir();
    virtual ~EstadoDesconectado() { }

private:
    void actualizar_estado_ui(bool habilitar);
};

#endif // _ESTADO_DESCONECTADO_H_
