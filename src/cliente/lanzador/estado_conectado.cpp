#include "cliente/lanzador/estado_conectado.h"

#include <QMessageBox>
#include <QStringList>

#include "cliente/lanzador/estado_desconectado.h"
#include "cliente/lanzador/estado_en_sala.h"

#include "ui_lanzador.h"

EstadoConectado::EstadoConectado(Ui::Lanzador *ui_, 
    cliente::Partida& partida_, cliente::Servidor*& servidor_) 
: EstadoLanzador(ui_, partida_, servidor_) { }

void EstadoConectado::entrar() {
    actualizar_estado_ui(true);
    ui->btnConectar->setText("Desconectar");
    ui->btnElegirSala->setText("UNIRSE");
    ui->lblNombreSala->setText("-");
    btn_actualizar_salas_click();
}

EstadoLanzador* EstadoConectado::btn_iniciar_juego_click() {
    QMessageBox msgBox;
    msgBox.setText("No estás en ninguna sala");
    msgBox.exec();
    return this;
}

EstadoLanzador* EstadoConectado::btn_conectar_click() {
    servidor->detener();
    delete servidor;
    servidor = nullptr;

    return new EstadoDesconectado(ui, partida, servidor);
}

EstadoLanzador* EstadoConectado::btn_actualizar_salas_click() {
    ui->listSalasDisponibles->clear();
    ui->cbMapas->clear();
    if (!servidor)
        return this;

    QStringList list_salas;

    for (const std::string& sala : servidor->obtener_salas()) {
        list_salas << sala.c_str();
    }
    
    ui->listSalasDisponibles->addItems(list_salas);

    QStringList list_mapas;

    for (const std::string& mapa : servidor->obtener_mapas()) {
        list_mapas << mapa.c_str();
    }

    ui->cbMapas->addItems(list_mapas);
    return this;
}

EstadoLanzador* EstadoConectado::btn_crear_sala_click() {
    QString qnombre = ui->txtNombreSala->text();
    QString qmapa = ui->cbMapas->currentText();
    
    if (qnombre == "") {
        QMessageBox msgBox;
        msgBox.setText("Ingrese un nombre para la sala");
        msgBox.exec();
        return this;
    }

    if (qmapa == "") {
        QMessageBox msgBox;
        msgBox.setText("Seleccione un mapa");
        msgBox.exec();
        return this;
    }

    std::string nombre = qnombre.toUtf8().constData();
    std::string mapa = qmapa.toUtf8().constData();

    if (!servidor->crear_sala(nombre, mapa)) {
        QMessageBox msgBox;
        msgBox.setText("No se pudo crear la sala");
        msgBox.exec();
        return this;
    }

    partida.nombre_sala(nombre);
    partida.mapa(mapa);

    return new EstadoEnSala(ui, partida, servidor);
}

EstadoLanzador* EstadoConectado::btn_elegir_salas_click() {
    QList<QListWidgetItem*> seleccion = 
        ui->listSalasDisponibles->selectedItems();
    
    if (seleccion.size() != 1) {
        QMessageBox msgBox;
        msgBox.setText("Debe elegir una (y sólo una) sala");
        msgBox.exec();
        return this;
    }

    std::string sala = seleccion.takeFirst()->text().toUtf8().constData();

    if (!servidor->unirse_a_sala(sala)) {
        QMessageBox msg_box;
        msg_box.setText("No se pudo unir a la sala");
        msg_box.exec();
        return this;
    }

    partida.nombre_sala(sala);
    return new EstadoEnSala(ui, partida, servidor);
}

void EstadoConectado::salir() {
    ui->lblNombreSala->setText("DESCONECTADO");
    actualizar_estado_ui(false);
}

void EstadoConectado::actualizar_estado_ui(bool habilitar) {
    ui->btnElegirSala->setEnabled(habilitar);
    ui->btnActualizar->setEnabled(habilitar);
    ui->listSalasDisponibles->setEnabled(habilitar);
    ui->cbMapas->setEnabled(habilitar);
    ui->txtNombreSala->setEnabled(habilitar);
    ui->btnCrearSala->setEnabled(habilitar);
}