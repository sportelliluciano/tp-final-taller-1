#ifndef _VENTANA_JUEGO_TERMINADO_H_
#define _VENTANA_JUEGO_TERMINADO_H_

#include <string>

#include <QWidget>

#include "cliente/lanzador/ui_ventana_juego_terminado.fwd.h"

class VentanaJuegoTerminado : public QWidget {
    Q_OBJECT

public:
    VentanaJuegoTerminado(const std::string& ganador, QWidget *parent = 0);
    virtual ~VentanaJuegoTerminado();

private:
    Ui::VentanaJuegoTerminado *ui;
};


#endif // _VENTANA_JUEGO_TERMINADO_H_