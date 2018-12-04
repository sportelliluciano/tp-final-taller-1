#include "cliente/lanzador/ventana_juego_terminado.h"

#include <string>

#include "ui_ventana_juego_terminado.h"

VentanaJuegoTerminado::VentanaJuegoTerminado(const std::string& ganador, 
    QWidget *parent)
: QWidget(parent), ui(new Ui::VentanaJuegoTerminado)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    ui->lblNombreGanador->setText(("GANADOR: " + ganador).c_str());
}

VentanaJuegoTerminado::~VentanaJuegoTerminado() {
    delete ui;
}
