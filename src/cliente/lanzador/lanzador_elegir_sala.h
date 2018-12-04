#ifndef _LANZADOR_ELEGIR_SALA_H_
#define _LANZADOR_ELEGIR_SALA_H_

#include <QWidget>

#include "cliente/cliente_juego.h"
#include "cliente/red/servidor_protegido.h"

#include "cliente/lanzador/ui_lanzador_elegir_sala.fwd.h"

class LanzadorElegirSala : public QWidget {
    Q_OBJECT

public:
    LanzadorElegirSala(cliente::Partida& nueva_partida, 
        cliente::ServidorProtegido& servidor_, QWidget *parent = 0);
    
    /**
     * \brief Debe ser llamado por el padre al mostrar este widget.
     */
    void entrar();
    
    virtual ~LanzadorElegirSala();

private:
    Ui::LanzadorElegirSala *ui;
    cliente::Partida& partida;
    cliente::ServidorProtegido* servidor;

    /**
     * \brief Conecta los botones de la interfaz con sus funciones asociadas.
     */
    void conectar_eventos();

    /**
     * \brief Los siguientes son los manejadores de la interfaz gráfica.
     * 
     */
    void btn_actualizar_click();
    void btn_unirse_click();
    void btn_crear_sala_click();

    bool crear_sala(const QString& nombre, const QString& mapa);
    bool unirse_a_sala(const QString& nombre);

signals:
    void listo();
};


#endif // _LANZADOR_ELEGIR_SALA_H_