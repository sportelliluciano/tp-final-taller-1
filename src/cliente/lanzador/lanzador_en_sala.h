#ifndef _LANZADOR_EN_SALA_H_
#define _LANZADOR_EN_SALA_H_

#include <QWidget>

#include "cliente/cliente_juego.h"
#include "cliente/red/servidor_protegido.h"

#include "cliente/lanzador/ui_lanzador_en_sala.fwd.h"

class LanzadorEnSala : public QWidget {
    Q_OBJECT

public:
    LanzadorEnSala(cliente::Partida& nueva_partida, 
        cliente::ServidorProtegido& servidor_, QWidget *parent = 0);
    
    /**
     * \brief Debe ejecutarse antes de mostrar el widget.
     */
    void entrar();
    
    virtual ~LanzadorEnSala();

public slots:
    void q_jugador_conectado(QString nombre, QString casa, bool listo);


private:
    Ui::LanzadorEnSala *ui;
    cliente::Partida& partida;
    cliente::ServidorProtegido* servidor;

    /**
     * \brief Conecta los botones de la interfaz con sus funciones asociadas.
     */
    void conectar_eventos();

    /**
     * \brief Los siguientes son los manejadores de la interfaz gráfica.
     * 
     * Se ejecuta al hacer clic en los botones.
     */
    void btn_atreides_click();
    void btn_harkonnen_click();
    void btn_ordos_click();
    void btn_actualizar_jugadores_click();
    void btn_iniciar_juego_click();


    void vaciar_lista_jugadores();
    void actualizar_lista_jugadores();

signals:
    void listo();
};


#endif // _LANZADOR_EN_SALA_H_