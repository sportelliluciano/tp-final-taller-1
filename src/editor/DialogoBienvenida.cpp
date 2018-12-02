#include "DialogoBienvenida.h"
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include "Editor.h"
#define MINIMO_CANTIDAD_JUGADORES 2
#define DIMENSION_MINIMA_MAPA 10
#define DIMENSION_MAXIMA_MAPA 1000
using std::string;

DialogoBienvenida::DialogoBienvenida(QWidget *parent) : editor_fue_creado(false), 
    QDialog(parent) {
    this->form_layout = new QFormLayout(this);

    this->titulo = new QLabel("Bienvenido al editor de mapas de Dune");
    form_layout->addRow(titulo);

    this->boton_crear_mapa = new QPushButton("CREAR MAPA");
    this->form_layout->addRow(this->boton_crear_mapa);
    QObject::connect(this->boton_crear_mapa, &QPushButton::clicked,
        this, &DialogoBienvenida::mostrar_dialogo_crear_mapa);

    this->boton_cargar_mapa = new QPushButton("CARGAR MAPA");
    this->form_layout->addRow(this->boton_cargar_mapa);
    QObject::connect(this->boton_cargar_mapa, &QPushButton::clicked,
        this, &DialogoBienvenida::mostrar_dialogo_cargar_mapa);

    this->setWindowTitle("Editor de Mapas de Dune");
}

void DialogoBienvenida::mostrar_dialogo_crear_mapa() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QString descripcion_filas ("Filas");
    QSpinBox spinbox_filas (&dialog);
    spinbox_filas.setMinimum(DIMENSION_MINIMA_MAPA);
    spinbox_filas.setMaximum(DIMENSION_MAXIMA_MAPA);
    form_layout.addRow(descripcion_filas, &spinbox_filas);

    QString descripcion_columnas ("Columnas");
    QSpinBox spinbox_columnas (&dialog);
    spinbox_columnas.setMinimum(DIMENSION_MINIMA_MAPA);
    spinbox_columnas.setMaximum(DIMENSION_MAXIMA_MAPA);
    form_layout.addRow(descripcion_columnas, &spinbox_columnas);

    QString descripcion_cant_jugadores ("Cantidad de jugadores");
    QSpinBox spinbox_jugadores (&dialog);
    spinbox_jugadores.setMinimum(MINIMO_CANTIDAD_JUGADORES);
    form_layout.addRow(descripcion_cant_jugadores, &spinbox_jugadores);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Crear mapa");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        int filas = spinbox_filas.value();
        int columnas = spinbox_columnas.value();
        int cantidad_de_jugadores = spinbox_jugadores.value();
        
        this->editor = new Editor (filas, columnas, cantidad_de_jugadores);
        this->editor_fue_creado = true;
        this->editor->show();

        // cierro DialogoBienvenida
        this->close();
    }
}

void DialogoBienvenida::mostrar_dialogo_cargar_mapa() {
    // filename, incluye el filepath completo.
    QString filename = QFileDialog::getOpenFileName(this, 
        "Cargar mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar o no eligio ningun archivo json.
        return;
    }
    
    // cargar mapa
    string filename_std_string = filename.toStdString();
    this->editor = new Editor (filename_std_string);
    this->editor_fue_creado = true;

    // cierro DialogoBienvenida y muestro el Editor.
    this->close();
    this->editor->show();
}

DialogoBienvenida::~DialogoBienvenida() {
    if (this->editor_fue_creado) {
        delete this->editor;
    }

    delete this->form_layout;
    delete this->titulo;
    delete this->boton_crear_mapa;
    delete this->boton_cargar_mapa;
}
