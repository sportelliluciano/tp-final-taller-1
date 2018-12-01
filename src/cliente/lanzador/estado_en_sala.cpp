#include "cliente/lanzador/estado_en_sala.h"

#include <QMessageBox>

#include "cliente/lanzador/estado_conectado.h"
#include "cliente/lanzador/estado_desconectado.h"

#include "ui_lanzador.h"

EstadoEnSala::EstadoEnSala(Ui::Lanzador *ui_, 
    cliente::Partida& partida_, cliente::Servidor*& servidor_) 
: EstadoLanzador(ui_, partida_, servidor_) { }

void EstadoEnSala::entrar() {
    ui->btnElegirSala->setText("DEJAR SALA");
    ui->lblNombreSala->setText(partida.nombre_sala().c_str());
    actualizar_estado_ui(true);
}

EstadoLanzador* EstadoEnSala::btn_conectar_click() {
    servidor->detener();
    delete servidor;
    servidor = nullptr;

    return new EstadoDesconectado(ui, partida, servidor);
}

EstadoLanzador* EstadoEnSala::btn_iniciar_juego_click() {
    std::string casa = "atreides";
    if (ui->rbHarkonnen->isChecked())
        casa = "harkonnen";
    else if (ui->rbOrdos->isChecked())
        casa = "ordos";
    
    const std::string& nombre = 
        ui->txtNombreJugador->text().toUtf8().constData();
    
    if (!servidor->avisar_jugador_listo(nombre, casa)) {
        QMessageBox msg_box;
        msg_box.setText("No se pudo iniciar la partida");
        msg_box.exec();
        return this;
    }
    partida.casa(casa);
    partida.servidor(servidor);
    partida.musica(ui->cbMusica->isChecked());
    partida.sonido(ui->cbSFX->isChecked());
    partida.pantalla_completa(ui->cbPantallaCompleta->isChecked());
    partida.vsync(ui->cbVsync->isChecked());
    partida.partida_lista(true);
    return nullptr;
}

EstadoLanzador* EstadoEnSala::btn_elegir_salas_click() {
    servidor->dejar_sala();
    return new EstadoConectado(ui, partida, servidor);
}

void EstadoEnSala::salir() {
    ui->lblNombreSala->setText("-");
    actualizar_estado_ui(false);
}

void EstadoEnSala::actualizar_estado_ui(bool habilitar) {
    ui->btnElegirSala->setEnabled(habilitar);
    ui->rbAtreides->setEnabled(habilitar);
    ui->rbHarkonnen->setEnabled(habilitar);
    ui->rbOrdos->setEnabled(habilitar);
    ui->listJugadoresEnSala->setEnabled(habilitar);
    ui->btnIniciarJuego->setEnabled(habilitar);
}