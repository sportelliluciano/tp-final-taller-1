#include "cliente/lanzador/ventana_principal.h"

#include <QWidget>
#include <QStackedLayout>
#include <QMessageBox>

#include "cliente/lanzador/lanzador.h"
#include "cliente/lanzador/lanzador_elegir_sala.h"
#include "cliente/lanzador/lanzador_en_sala.h"

VentanaPrincipal::VentanaPrincipal(cliente::Partida& partida, QWidget *parent)
: QWidget(parent), layout(new QStackedLayout(this)), 
  lanzador(partida, servidor, this), 
  elegir_sala(partida, servidor, this), 
  en_sala(partida, servidor, this)
{
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setWindowTitle(tr("Dune Remake"));
    
    layout->addWidget(&lanzador);
    layout->addWidget(&elegir_sala);
    layout->addWidget(&en_sala);
    layout->setCurrentIndex(0);

    connect(&lanzador, &Lanzador::listo, this, 
        &VentanaPrincipal::ir_a_elegir_sala);
    connect(&elegir_sala, &LanzadorElegirSala::listo, this, 
        &VentanaPrincipal::ir_a_en_sala);
    connect(&en_sala, &LanzadorEnSala::listo, this, 
        &VentanaPrincipal::terminar);
}

void VentanaPrincipal::ir_a_elegir_sala() {
    elegir_sala.entrar();
    layout->setCurrentIndex(1);
}

void VentanaPrincipal::ir_a_en_sala() {
    en_sala.entrar();
    layout->setCurrentIndex(2);
}

void VentanaPrincipal::terminar() {
    close();
}
