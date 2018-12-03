#include "cliente/lanzador/lanzador.h"

#include <string>

#include <QMessageBox>
#include <QStringList>

#include "cliente/red/partida.h"
#include "cliente/lanzador/estado_desconectado.h"

#include "ui_lanzador.h"

Lanzador::Lanzador(cliente::Partida& nueva_partida, QWidget *parent) 
: QWidget(parent), ui(new Ui::Lanzador), partida(nueva_partida), 
  servidor(nullptr)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    conectar_eventos();
    estado = new EstadoDesconectado(ui, partida, servidor);
    estado->entrar();
}

void Lanzador::conectar_eventos() {
    QObject::connect(ui->btnIniciarJuego, &QPushButton::clicked, this,
        &Lanzador::btn_iniciar_juego_click);

    QObject::connect(ui->btnConectar, &QPushButton::clicked, this, 
        &Lanzador::btn_conectar_click);

    QObject::connect(ui->btnActualizar, &QPushButton::clicked, this,
        &Lanzador::btn_actualizar_salas_click);
    
    QObject::connect(ui->btnCrearSala, &QPushButton::clicked, this,
        &Lanzador::btn_crear_sala_click);
    
    QObject::connect(ui->btnElegirSala, &QPushButton::clicked, this,
        &Lanzador::btn_elegir_salas_click);
    
    QObject::connect(ui->listSalasDisponibles, &QListWidget::clicked, 
            this, &Lanzador::list_salas_item_clicked);
}

Lanzador::~Lanzador() {
    delete ui;
    if (estado)
        delete estado;
}

void Lanzador::list_salas_item_clicked() {
    cambiar_estado(estado->list_salas_item_clicked());
}

void Lanzador::btn_iniciar_juego_click() {
    cambiar_estado(estado->btn_iniciar_juego_click());
}

void Lanzador::btn_actualizar_salas_click() {
    cambiar_estado(estado->btn_actualizar_salas_click());
}

void Lanzador::btn_conectar_click() {
    cambiar_estado(estado->btn_conectar_click());   
}

void Lanzador::btn_elegir_salas_click() {
    cambiar_estado(estado->btn_elegir_salas_click());
}

void Lanzador::btn_crear_sala_click() {
    cambiar_estado(estado->btn_crear_sala_click());
}

void Lanzador::cambiar_estado(EstadoLanzador* nuevo_estado) {
    if (nuevo_estado == nullptr) {
        delete estado;
        estado = nullptr;
        close();
        return;
    }

    if (nuevo_estado != estado) {
        estado->salir();
        delete estado;
        estado = nuevo_estado;
        estado->entrar();
    }
}
