#include "cliente/lanzador/lanzador_elegir_sala.h"

#include <string>

#include <QMessageBox>
#include <QStringList>

#include "cliente/red/partida.h"
#include "cliente/red/servidor_protegido.h"

#include "ui_lanzador_elegir_sala.h"

LanzadorElegirSala::LanzadorElegirSala(cliente::Partida& nueva_partida, 
    cliente::ServidorProtegido& servidor_, QWidget *parent) 
: QWidget(parent), ui(new Ui::LanzadorElegirSala), partida(nueva_partida),
  servidor(&servidor_)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    conectar_eventos();
}

void LanzadorElegirSala::conectar_eventos() {
    QObject::connect(ui->btnActualizar, &QPushButton::clicked, this, 
        &LanzadorElegirSala::btn_actualizar_click);
    QObject::connect(ui->btnUnirse, &QPushButton::clicked, this, 
        &LanzadorElegirSala::btn_unirse_click);
    QObject::connect(ui->btnCrearSala, &QPushButton::clicked, this, 
        &LanzadorElegirSala::btn_crear_sala_click);
    
    QObject::connect(ui->txtNombreSala, &QLineEdit::returnPressed, this,
        &LanzadorElegirSala::btn_crear_sala_click);
}

void LanzadorElegirSala::entrar() {
    btn_actualizar_click();
}

void LanzadorElegirSala::btn_actualizar_click() {
    ui->listSalasDisponibles->clear();
    ui->cbMapas->clear();
    
    QStringList list_salas;

    for (const std::string& sala : servidor->obtener_nombres_salas()) {
        list_salas << sala.c_str();
    }
    
    ui->listSalasDisponibles->addItems(list_salas);

    QStringList list_mapas;

    for (const std::string& mapa : servidor->obtener_nombres_mapas()) {
        list_mapas << mapa.c_str();
    }

    ui->cbMapas->addItems(list_mapas);
}

void LanzadorElegirSala::btn_unirse_click() {
    QList<QListWidgetItem*> seleccion = 
        ui->listSalasDisponibles->selectedItems();
    
    if (seleccion.size() != 1) {
        QMessageBox::warning(this, "Dune Remake", 
            "Debe elegir una (y sólo una) sala");
        return;
    }

    QString sala = seleccion.takeFirst()->text();

    if (!unirse_a_sala(sala)) {
        QMessageBox::warning(this, "Dune Remake", "No se pudo unir a la sala");
        return;
    }

    emit listo();
}

void LanzadorElegirSala::btn_crear_sala_click() {
    QString nombre = ui->txtNombreSala->text().trimmed();
    QString mapa = ui->cbMapas->currentText();
    
    if (nombre.isEmpty() || mapa.isEmpty()) {
        QMessageBox::warning(this, "Dune Remake", 
            "Debe ingresar un nombre de sala y elegir un mapa.");
        return;
    }

    if (!crear_sala(nombre, mapa)) {
        QMessageBox::critical(this, "Dune Remake", "No se pudo crear la sala.");
        return;
    }

    emit listo();
}

bool LanzadorElegirSala::crear_sala(const QString& nombre, const QString& mapa) 
{
    try {
        std::string s_nombre = nombre.toUtf8().constData();
        std::string s_mapa = mapa.toUtf8().constData();
        
        if (!servidor->crear_sala(s_nombre, s_mapa))
            return false;
            
        partida.nombre_sala(s_nombre);
        partida.mapa(s_mapa);
    } catch (const std::exception&) {
        return false;
    }
    return true;
}

bool LanzadorElegirSala::unirse_a_sala(const QString& nombre) {
    try {
        std::string s_nombre = nombre.toUtf8().constData();
        
        if (!servidor->unirse_a_sala(s_nombre))
            return false;
        partida.nombre_sala(s_nombre);
    } catch (const std::exception&) {
        return false;
    }
    return true;
}


LanzadorElegirSala::~LanzadorElegirSala() { 
    delete ui;
}
