#ifndef _ESTADO_EN_SALA_H_
#define _ESTADO_EN_SALA_H_

#include "cliente/lanzador/estado_lanzador.h"

class EstadoEnSala : public EstadoLanzador {
public:
    EstadoEnSala(Ui::Lanzador *ui_, cliente::Partida& partida_, 
        cliente::Servidor*& servidor_);
    virtual void entrar();
    EstadoLanzador* btn_iniciar_juego_click() override;
    EstadoLanzador* btn_conectar_click() override;
    EstadoLanzador* btn_elegir_salas_click() override;
    virtual void salir();
    virtual ~EstadoEnSala() { }

private:
    void actualizar_estado_ui(bool habilitar);
};

#endif // _ESTADO_EN_SALA_H_
