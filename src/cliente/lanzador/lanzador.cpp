#include "cliente/lanzador/lanzador.h"

#include <string>

#include <QMessageBox>
#include <QStringList>

#include "cliente/partida.h"

#include "ui_lanzador.h"

Lanzador::Lanzador(cliente::Partida& nueva_partida, QWidget *parent) 
: QWidget(parent), partida(nueva_partida), ui(new Ui::Lanzador), 
  servidor(nullptr)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    conectar_eventos();
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
}

void Lanzador::list_salas_item_clicked() {
    ui->btnElegirSala->setEnabled(true);
}

void Lanzador::btn_iniciar_juego_click() {
    if (!servidor) {
        QMessageBox msgBox;
        msgBox.setText("No estás conectado");
        msgBox.exec();
        return;
    }
    servidor->avisar_jugador_listo();
    partida.servidor(servidor);
    partida.musica(ui->cbMusica->isChecked());
    partida.sonido(ui->cbSFX->isChecked());
    partida.pantalla_completa(ui->cbPantallaCompleta->isChecked());
    partida.vsync(ui->cbVsync->isChecked());
    partida.partida_lista(true);
    close();
}

void Lanzador::btn_actualizar_salas_click() {
    ui->listSalasDisponibles->clear();
    ui->cbMapas->clear();
    if (!servidor)
        return;

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
}

void Lanzador::btn_conectar_click() {
    ui->btnConectar->setEnabled(false);
    if (servidor) {
        servidor->detener();
        delete servidor;
        servidor = nullptr;
        
        ui->btnConectar->setEnabled(true);
        ui->btnConectar->setText("Conectar");
        ui->txtDireccionServidor->setEnabled(true);
        ui->txtPuertoServidor->setEnabled(true);

        ui->btnActualizar->setEnabled(false);
        ui->listSalasDisponibles->setEnabled(false);
        ui->cbMapas->setEnabled(false);
        ui->btnElegirSala->setEnabled(false);
        ui->txtNombreSala->setEnabled(false);
        ui->btnCrearSala->setEnabled(false);
    } else {
        try {
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
            ui->btnConectar->setEnabled(true);
            return;
        }

        ui->btnConectar->setEnabled(true);
        ui->btnConectar->setText("Desconectar");
        ui->txtDireccionServidor->setEnabled(false);
        ui->txtPuertoServidor->setEnabled(false);

        ui->btnActualizar->setEnabled(true);
        ui->listSalasDisponibles->setEnabled(true);
        ui->cbMapas->setEnabled(true);
        ui->txtNombreSala->setEnabled(true);
        ui->btnCrearSala->setEnabled(true);
        btn_actualizar_salas_click();
    }
}

void Lanzador::btn_elegir_salas_click() {
    if (esta_en_sala) {
        servidor->dejar_sala();
        al_dejar_sala();
    } else {
        QList<QListWidgetItem*> seleccion = 
            ui->listSalasDisponibles->selectedItems();
        
        if (seleccion.size() != 1) {
            QMessageBox msgBox;
            msgBox.setText("Debe elegir una (y sólo una) sala");
            msgBox.exec();
            return;
        }

        std::string sala = seleccion.takeFirst()->text().toUtf8().constData();
        nombre_sala = sala;

        servidor->unirse_a_sala(sala);
        al_ingresar_sala();
    }
}

void Lanzador::btn_crear_sala_click() {
    if (esta_en_sala) {
        QMessageBox msgBox;
        msgBox.setText("Debe salir de la sala actual para crear una nueva");
        msgBox.exec();
        return;
    }
    
    QString qnombre = ui->txtNombreSala->text();
    QString qmapa = ui->cbMapas->currentText();
    
    if (qnombre == "") {
        QMessageBox msgBox;
        msgBox.setText("Ingrese un nombre para la sala");
        msgBox.exec();
        return;
    }

    if (qmapa == "") {
        QMessageBox msgBox;
        msgBox.setText("Seleccione un mapa");
        msgBox.exec();
        return;
    }

    std::string nombre = qnombre.toUtf8().constData();
    std::string mapa = qmapa.toUtf8().constData();

    if (!servidor->crear_sala(nombre, mapa)) {
        QMessageBox msgBox;
        msgBox.setText("No se pudo crear la sala");
        msgBox.exec();
        return;
    }

    nombre_sala = nombre;
    al_ingresar_sala();
}

void Lanzador::al_ingresar_sala() {
    btn_actualizar_salas_click();
    ui->lblNombreSala->setText(nombre_sala.c_str());
    ui->listSalasDisponibles->setEnabled(false);
    ui->cbMapas->setEnabled(false);
    ui->txtNombreSala->setEnabled(false);
    ui->btnCrearSala->setEnabled(false);
    ui->btnActualizar->setEnabled(false);
    ui->btnElegirSala->setEnabled(true);
    ui->btnElegirSala->setText("DEJAR SALA");
    esta_en_sala = true;

    ui->txtNombreJugador->setEnabled(true);
    ui->rbAtreides->setEnabled(true);
    ui->rbHarkonnen->setEnabled(true);
    ui->rbOrdos->setEnabled(true);
    ui->listJugadoresEnSala->setEnabled(true);
    ui->btnIniciarJuego->setEnabled(true);
}

void Lanzador::al_dejar_sala() {
    btn_actualizar_salas_click();
    ui->lblNombreSala->setText("-");
    ui->btnActualizar->setEnabled(true);
    ui->listSalasDisponibles->setEnabled(true);
    ui->cbMapas->setEnabled(true);
    ui->txtNombreSala->setEnabled(true);
    ui->btnCrearSala->setEnabled(true);
    ui->btnElegirSala->setText("UNIRSE");
    esta_en_sala = false;

    ui->txtNombreJugador->setEnabled(false);
    ui->rbAtreides->setEnabled(false);
    ui->rbHarkonnen->setEnabled(false);
    ui->rbOrdos->setEnabled(false);
    ui->listJugadoresEnSala->setEnabled(false);
    ui->btnIniciarJuego->setEnabled(false);
}
