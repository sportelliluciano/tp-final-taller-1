#ifndef _VENTANA_PRINCIPAL_H_
#define _VENTANA_PRINCIPAL_H_

#include <QWidget>
#include <QStackedLayout>

#include "cliente/lanzador/lanzador.h"
#include "cliente/lanzador/lanzador_elegir_sala.h"
#include "cliente/lanzador/lanzador_en_sala.h"

#include "cliente/red/servidor_protegido.h"

class VentanaPrincipal : public QWidget {
    Q_OBJECT

public:
    explicit VentanaPrincipal(cliente::Partida& partida, QWidget *parent = 0);

private:
    QStackedLayout *const layout;
    Lanzador lanzador;
    LanzadorElegirSala elegir_sala;
    LanzadorEnSala en_sala;
    cliente::ServidorProtegido servidor;

public slots:
    void ir_a_elegir_sala();
    void ir_a_en_sala();
    void terminar();
};

#endif // _VENTANA_PRINCIPAL_H_
