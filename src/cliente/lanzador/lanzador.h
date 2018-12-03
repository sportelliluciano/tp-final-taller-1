#ifndef _LANZADOR_H_
#define _LANZADOR_H_

#include <memory>

#include <QWidget>
#include <QStringList>

#include "cliente/cliente_juego.h"
#include "cliente/red/servidor.h"
#include "cliente/lanzador/estado_lanzador.h"

#include "cliente/lanzador/ui_lanzador.fwd.h"

class Lanzador : public QWidget {
    Q_OBJECT

public:
    Lanzador(cliente::Partida& nueva_partida, QWidget *parent = 0);
    virtual ~Lanzador();

private:
    Ui::Lanzador *ui;
    cliente::Partida& partida;
    cliente::Servidor* servidor;

    /**
     * \brief Conecta los botones de la interfaz con sus funciones asociadas.
     */
    void conectar_eventos();

    /**
     * \brief Los siguientes son los manejadores de la interfaz gráfica.
     * 
     * btn_*_click: Se ejecuta al hacer click en el botón *.
     * list_salas_item_clicked: Se ejecuta al elegir una sala de la lista.
     */
    void btn_iniciar_juego_click();
    void btn_conectar_click();
    void btn_actualizar_salas_click();
    void btn_elegir_salas_click();
    void btn_crear_sala_click();
    void list_salas_item_clicked();

    /**
     * \brief Cambia el estado actual del lanzador si es necesario.
     * 
     * Pasar como estado un puntero nulo cerrará el lanzador.
     */
    void cambiar_estado(EstadoLanzador* nuevo_estado);

    EstadoLanzador* estado;
};


#endif // _LANZADOR_H_