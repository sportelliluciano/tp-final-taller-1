#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    // Clase que contiene el loop principal
    QApplication app(argc, argv);
    // Instancio un widget abstracto padre
    QWidget parent;
    // Creo un layout para que disponga a los widgets en forma vertical
    QVBoxLayout layout(&parent);
    // Creo los widgets
    QLabel label1("Label 1");
    QLabel label2("Label 2");
    QPushButton button("Button");
    // Agrego los widgets al layout vertical
    layout.addWidget(&label1);
    layout.addWidget(&button);
    layout.addWidget(&label2);
    // Muestro el elemento y sus hijos
    parent.show();
    // Arranca el loop de la UI
    return app.exec();
}
