#ifndef _LANZADOR_H_
#define _LANZADOR_H_

#include <QWidget>

#include "cliente/red/partida.h"
#include "cliente/red/servidor_protegido.h"

#include "cliente/lanzador/ui_lanzador.fwd.h"

class Lanzador : public QWidget {
    Q_OBJECT

public:
    Lanzador(cliente::Partida& nueva_partida, 
        cliente::ServidorProtegido& servidor, QWidget *parent = 0);
    virtual ~Lanzador();

private:
    Ui::Lanzador *ui;
    cliente::Partida& partida;
    cliente::ServidorProtegido* servidor;

    /**
     * \brief Conecta los botones de la interfaz con sus funciones asociadas.
     */
    void conectar_eventos();

    /**
     * \brief Los siguientes son los manejadores de la interfaz gráfica.
     * 
     * Se ejecuta al hacer clic en el botón de conectar.
     */
    void btn_conectar_click();

    /**
     * \brief Intenta conectarse al servidor.
     * 
     * Si la conexión es exitosa configura el servidor en la partida y 
     * devuelve true. En caso contrario devuelve false.
     */
    bool conectar(const QString& direccion, const QString& puerto,
        const QString& nombre);

signals:
    void listo();
};


#endif // _LANZADOR_H_