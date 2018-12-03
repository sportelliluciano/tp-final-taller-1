#include "cliente/lanzador/lanzador.h"

#include <string>

#include <QMessageBox>
#include <QStringList>
#include <QStackedLayout>

#include "cliente/red/partida.h"
#include "cliente/red/servidor.h"
#include "cliente/red/servidor_protegido.h"

#include "ui_lanzador.h"

Lanzador::Lanzador(cliente::Partida& nueva_partida, 
        cliente::ServidorProtegido& servidor_, QWidget *parent)
: QWidget(parent), ui(new Ui::Lanzador), partida(nueva_partida), 
  servidor(&servidor_)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    conectar_eventos();
}

void Lanzador::conectar_eventos() {
    QObject::connect(ui->btnConectar, &QPushButton::clicked, this, 
        &Lanzador::btn_conectar_click);
    
    QObject::connect(ui->txtDireccionServidor, &QLineEdit::returnPressed,
        this, &Lanzador::btn_conectar_click);
    QObject::connect(ui->txtPuertoServidor, &QLineEdit::returnPressed,
        this, &Lanzador::btn_conectar_click);
    QObject::connect(ui->txtNombreJugador, &QLineEdit::returnPressed,
        this, &Lanzador::btn_conectar_click);
}

Lanzador::~Lanzador() {
    delete ui;
}

void Lanzador::btn_conectar_click() {
    QString direccion = ui->txtDireccionServidor->text().trimmed(),
            puerto = ui->txtPuertoServidor->text().trimmed(),
            nombre = ui->txtNombreJugador->text().trimmed();
    
    if ((direccion.isEmpty()) || (puerto.isEmpty()) || (nombre.isEmpty())) {
        QMessageBox msg_box;
        msg_box.setWindowTitle("Dune Remake");
        msg_box.setText("Debe ingresar todos los datos");
        msg_box.setIcon(QMessageBox::Warning);
        msg_box.exec();
        return;
    }

    if (!conectar(direccion, puerto, nombre)) {
        QMessageBox msg_box;
        msg_box.setWindowTitle("Dune Remake");
        msg_box.setText("No se pudo conectar al servidor.");
        msg_box.setInformativeText("Verifique los datos y vuelva a intentarlo");
        msg_box.setIcon(QMessageBox::Critical);
        msg_box.exec();
        return;
    }

    emit listo();
}

bool Lanzador::conectar(const QString& direccion, const QString& puerto, 
    const QString& nombre) 
{
    cliente::Servidor* nuevo_servidor = nullptr;
    try {
        nuevo_servidor = new cliente::Servidor(direccion.toUtf8().constData(), 
            puerto.toUtf8().constData());
    } catch (const std::exception& e) {
        return false;
    }
    
    partida.nombre_jugador(nombre.toUtf8().constData());
    partida.servidor(nuevo_servidor);
    servidor->set_servidor(nuevo_servidor);
    servidor->set_nombre(nombre.toUtf8().constData());
    return true;
}