#include "cliente/lanzador/lanzador_en_sala.h"

#include <string>

#include <QMessageBox>
#include <QStringList>
#include <QFrame>
#include <QTimer>

#include "cliente/red/partida.h"
#include "cliente/red/servidor_protegido.h"
#include "cliente/lanzador/jugador.h"

#include "ui_lanzador_en_sala.h"

#define CSS_BASE(color, img) "QWidget#wxContenedor { " \
    "background-repeat: none; " \
    "background-position: center; " \
    "background-image: url(:/imgs/nuevos/" img ".png); " \
    "background-color: #" color "; " \
"}"

#define CSS_ATREIDES  CSS_BASE("000033", "atreides")
#define CSS_HARKONNEN CSS_BASE("550000", "harkonnen")
#define CSS_ORDOS     CSS_BASE("004100", "ordos")
        

LanzadorEnSala::LanzadorEnSala(cliente::Partida& nueva_partida, 
    cliente::ServidorProtegido& servidor_, QWidget *parent)
: QWidget(parent), ui(new Ui::LanzadorEnSala), partida(nueva_partida),
  servidor(&servidor_)
{
    Q_INIT_RESOURCE(recursos);
    ui->setupUi(this);
    conectar_eventos();
    vaciar_lista_jugadores();
}

void LanzadorEnSala::entrar() {
    // Por defecto en el servidor el usuario es de Ordos.
    // TODO: Des-hardcodear esto
    btn_ordos_click();
    actualizar_lista_jugadores();

    ui->lblNombreJugador->setText(partida.nombre_jugador().c_str());
    ui->lblNombreSala->setText(("SALA: " + partida.nombre_sala()).c_str());
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, 
        this, &LanzadorEnSala::actualizar_lista_jugadores);
    timer->start(10000);
}

void LanzadorEnSala::conectar_eventos() {
    QObject::connect(ui->btnAtreides, &QPushButton::clicked, this, 
        &LanzadorEnSala::btn_atreides_click);
    QObject::connect(ui->btnHarkonnen, &QPushButton::clicked, this, 
        &LanzadorEnSala::btn_harkonnen_click);
    QObject::connect(ui->btnOrdos, &QPushButton::clicked, this, 
        &LanzadorEnSala::btn_ordos_click);
    
    QObject::connect(ui->btnIniciarJuego, &QPushButton::clicked, this, 
        &LanzadorEnSala::btn_iniciar_juego_click);

    QObject::connect(ui->btnActualizarJugadores, &QPushButton::clicked, this, 
        &LanzadorEnSala::btn_actualizar_jugadores_click);
}

LanzadorEnSala::~LanzadorEnSala() { 
    delete ui;
}

void LanzadorEnSala::btn_atreides_click() {
    ui->lblJugandoCon->setText("JUGANDO CON: ATREIDES");
    ui->wxContenedor->setStyleSheet(CSS_ATREIDES);
    servidor->set_casa("atreides");
    partida.casa("atreides");
    actualizar_lista_jugadores();
}

void LanzadorEnSala::btn_harkonnen_click() {
    ui->lblJugandoCon->setText("JUGANDO CON: HARKONNEN");
    ui->wxContenedor->setStyleSheet(CSS_HARKONNEN);
    servidor->set_casa("harkonnen");
    partida.casa("harkonnen");
    actualizar_lista_jugadores();
}

void LanzadorEnSala::btn_ordos_click() {
    ui->lblJugandoCon->setText("JUGANDO CON: ORDOS");
    ui->wxContenedor->setStyleSheet(CSS_ORDOS);
    servidor->set_casa("ordos");
    partida.casa("ordos");
    actualizar_lista_jugadores();
}

void LanzadorEnSala::btn_actualizar_jugadores_click() {
    actualizar_lista_jugadores();
}

void LanzadorEnSala::btn_iniciar_juego_click() {
    ui->btnIniciarJuego->setEnabled(false);
    ui->btnIniciarJuego->setText("Esperando...");
    partida.partida_lista(true);
    servidor->iniciar_juego();
    emit listo();
}

void LanzadorEnSala::actualizar_lista_jugadores() {
    vaciar_lista_jugadores();
    try {
        for (const nlohmann::json jug : 
            servidor->obtener_jugadores_sala(partida.nombre_sala())) 
        {
            q_jugador_conectado(
                jug.at("nombre").get<std::string>().c_str(),
                jug.at("casa").get<std::string>().c_str(),
                jug.at("listo")
            );
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Dune Remake", "Se perdió la conexión");
        close();
    }
}

void LanzadorEnSala::q_jugador_conectado(QString nombre, QString casa, 
    bool listo) 
{
    Jugador* jugador = new Jugador(ui->contenedorJugadores, 
        nombre.toUtf8().constData());
    ui->wxJugadores->addWidget(jugador);
    jugador->set_nombre(nombre.toUtf8().constData());
    jugador->set_casa(casa.toUtf8().constData());
    jugador->set_listo(listo);
}

void LanzadorEnSala::vaciar_lista_jugadores() {
    qDeleteAll(ui->contenedorJugadores->findChildren<QFrame*>(QString(), 
        Qt::FindDirectChildrenOnly));
}
