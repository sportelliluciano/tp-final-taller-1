#include "cliente/lanzador/estado_desconectado.h"

#include <QMessageBox>

#include "cliente/lanzador/estado_conectado.h"
#include "ui_lanzador.h"

EstadoDesconectado::EstadoDesconectado(Ui::Lanzador *ui_, 
    cliente::Partida& partida_, cliente::Servidor*& servidor_) 
: EstadoLanzador(ui_, partida_, servidor_) { }

void EstadoDesconectado::entrar() {
    actualizar_estado_ui(true);
    ui->btnConectar->setText("Conectar");
}

EstadoLanzador* EstadoDesconectado::btn_iniciar_juego_click() {
    QMessageBox msgBox;
    msgBox.setText("No estás conectado");
    msgBox.exec();
    return this;
}

EstadoLanzador* EstadoDesconectado::btn_conectar_click() {
    try {
        actualizar_estado_ui(false);
        servidor = new cliente::Servidor(
            ui->txtDireccionServidor->text().toUtf8().constData(),
            ui->txtPuertoServidor->text().toUtf8().constData()
        );
    } catch(const std::exception& e) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(QString("No se pudo conectar al servidor: ") 
            + QString(e.what()));
        msgBox.exec();
        actualizar_estado_ui(true);
        return this;
    }

    return new EstadoConectado(ui, partida, servidor);
}

void EstadoDesconectado::salir() {
    actualizar_estado_ui(false);
}

void EstadoDesconectado::actualizar_estado_ui(bool habilitar) {
    ui->txtDireccionServidor->setEnabled(habilitar);
    ui->txtPuertoServidor->setEnabled(habilitar);
    ui->txtNombreJugador->setEnabled(habilitar);
}