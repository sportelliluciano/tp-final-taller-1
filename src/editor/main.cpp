#include <QApplication>
#include "editor/dialogo_bienvenida.h"

int main(int argc, char *argv[]) {
    // Clase que contiene el loop principal
    QApplication app(argc, argv);
    
    // instancio Dialogo Bienvenida y lo muestro.
    DialogoBienvenida dialogo;
    dialogo.show();

    // Arranca el loop de la UI.
    return app.exec();
}
