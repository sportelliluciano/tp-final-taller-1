#ifndef _LANZADOR_H_
#define _LANZADOR_H_

#include <QWidget>
#include <QStringList>

#include "cliente/cliente_juego.h"
#include "cliente/servidor.h"

namespace Ui { class Lanzador; }

class Lanzador : public QWidget {
    Q_OBJECT

public:
    Lanzador(cliente::Partida& nueva_partida, QWidget *parent = 0);
    virtual ~Lanzador();

private:
    cliente::Partida& partida;
    Ui::Lanzador *ui;
    cliente::Servidor* servidor;

    void conectar_eventos();

    void btn_iniciar_juego_click();
    void btn_conectar_click();
    void btn_actualizar_salas_click();
    void btn_elegir_salas_click();
    void btn_crear_sala_click();
    void list_salas_item_clicked();
    void al_ingresar_sala();
    void al_dejar_sala();

    bool esta_en_sala = false;
    std::string nombre_sala;
};


#endif // _LANZADOR_H_