#include "editor/editor.h"
#include "ui_Editor.h"
#include <iostream>
#include <QMessageBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "editor/sprite.h"
#define MINIMO_CANTIDAD_JUGADORES 2
#define DIMENSION_MINIMA_MAPA 30
#define DIMENSION_MAXIMA_MAPA 1000
#define TIPO_JUGADOR 6
using std::string;

Editor::Editor(int filas, int columnas, int cant_jugadores, QWidget *parent) : 
    QMainWindow(parent, Qt::Window), mapa(Mapa(filas, columnas, this)), tabs(Tabs(this)),
    cant_elegida_jugadores(cant_jugadores), min_cant_jugadores(MINIMO_CANTIDAD_JUGADORES) {
    // Instancio la configuracion generada por el designer y uic
    Ui::Editor editor;
    // Configuro este widget para que use esa configuracion. A partir de aca
    // puedo usar findChild.
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild.
    this->tabs.inicializar_tabs();
    this->mapa.inicializar_mapa();

    // agrego al editor como observador del mapa.
    this->mapa.agregar_observador(this);

    inicializar_menu();
}


Editor::Editor(string& filename_json, QWidget *parent) : QMainWindow(parent, 
    Qt::Window), mapa(Mapa(this)), tabs(Tabs(this)) {
    // Instancio la configuracion generada por el designer y uic.
    Ui::Editor editor;
    // Configuro este widget para use esa configuracion. A partir de aca
    // puedo usar findChild.
    editor.setupUi(this);

    // aca inicializo Tabs y Mapa para poder utilizar findChild.
    this->tabs.inicializar_tabs();
    this->mapa.parsear_json(filename_json);

    // actualizo atributos segun el Mapa cargado.
    this->cant_elegida_jugadores = this->mapa.get_cantidad_jugadores_agregados();
    this->min_cant_jugadores = this->cant_elegida_jugadores;

    // agrego al editor como observador del mapa.
    this->mapa.agregar_observador(this);

    inicializar_menu();
}


void Editor::en_notificacion(string& id_label_mapa) {
    // getteo el sprite clickeado en Tabs.
    Sprite sprite_tab_clickeado = this->tabs.get_sprite_clickeado();

    // me fijo si hay algun Label clickeado en Tabs.
    if (sprite_tab_clickeado.id != "") {
        // me fijo si es el caso especial en que el tipo es Jugador.
        if (sprite_tab_clickeado.tipo == TIPO_JUGADOR) {
            // me fijo que el tipo de LabelMapa sea una Roca para poder apoyar
            // al jugador.
            if (this->mapa.es_valido_agregar_jugador(id_label_mapa, 
                this->cant_elegida_jugadores)) {
                // agrego al jugador al mapa
                this->mapa.agregar_jugador(id_label_mapa, sprite_tab_clickeado);
            } else {
                return;
            }
        } else {
            this->mapa.actualizar_data(id_label_mapa, sprite_tab_clickeado);
        }

        // actualizo el valor minimo de cantidad de jugadores
        int cantidad_jugadores_agregados = 
            this->mapa.get_cantidad_jugadores_agregados();
        if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
            this->min_cant_jugadores = MINIMO_CANTIDAD_JUGADORES;
        } else {
            this->min_cant_jugadores = cantidad_jugadores_agregados;
        }
    }
}


void Editor::inicializar_menu() {
    // conecto QActions del menu
    QAction* guardar_mapa = this->findChild<QAction*>("actionGuardar_mapa");
    connect(guardar_mapa, &QAction::triggered, this, &Editor::guardar_mapa);

    QAction* cargar_mapa = this->findChild<QAction*>("actionCargar_mapa");
    connect(cargar_mapa, &QAction::triggered, this, 
        &Editor::cargar_mapa_en_ejecucion);

    QAction* salir = this->findChild<QAction*>("actionSalir");
    connect(salir, &QAction::triggered, this, &Editor::terminar_ejecucion);

    QAction* cambiar_jugadores = this->findChild<QAction*>("actionCambiar_cantidad_de_jugadores");
    connect(cambiar_jugadores, &QAction::triggered, this, 
        &Editor::mostrar_dialogo_cantidad_jugadores);

    QAction* cambiar_tamanio_mapa = this->findChild<QAction*>("actionCambiar_tama_o_del_mapa");
    connect(cambiar_tamanio_mapa, &QAction::triggered, this, 
        &Editor::mostrar_dialogo_tamanio_mapa);
}

void Editor::cargar_mapa_en_ejecucion() {
    // filename, incluye el filepath completo.
    QString filename = QFileDialog::getOpenFileName(this, 
        "Cargar mapa", "", "JSON files (*.json)");
    if (filename.isNull()) {
        // toco cancelar o no eligio ningun archivo json.
        return;
    }
    
    // me fijo si el usuario incluyo la extension del archivo al ingresar el
    // nombre del mismo.
    if(!filename.contains(".json", Qt::CaseSensitive)) {
        filename += ".json";
    }

    // limpio el mapa actual.
    this->mapa.limpiar_mapa();

    // cargo el elegido por el user mapa.
    string filename_std_string = filename.toStdString();
    this->mapa.parsear_json(filename_std_string);

    // actualizo atributos.
    this->cant_elegida_jugadores = this->mapa.get_cantidad_jugadores_agregados();
    this->min_cant_jugadores = this->cant_elegida_jugadores;
}

void Editor::guardar_mapa() {
    int cantidad_jugadores_agregados = this->mapa.get_cantidad_jugadores_agregados();
    if (cantidad_jugadores_agregados != this->cant_elegida_jugadores) {
        QMessageBox::critical(this, "Error al guardar mapa", 
            "Falta agregar jugadores");
        return;
    }

    // muestra un dialogo para elegir el nombre del mapa y el directorio.
    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Guardar mapa",
        QDir::currentPath(), "JSON (*.json)");
    if (nombre_archivo.isNull()) {
        return;
    }
    
    // me fijo si el usuario incluyo la extension del archivo al ingresar el
    // nombre del mismo.
    if(!nombre_archivo.contains(".json", Qt::CaseSensitive)) {
        nombre_archivo += ".json";
    }
    string nombre_archivo_std_string = nombre_archivo.toStdString();
    this->mapa.generar_json(nombre_archivo_std_string);

    // muestro mensaje de que se guardo el mapa correctamente
    QMessageBox::information(this, "Mapa guardado", 
            "Se guardo el mapa correctamente.");
}


void Editor::mostrar_dialogo_tamanio_mapa() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    int filas = this->mapa.get_cant_filas();
    int columnas = this->mapa.get_cant_columnas();

    QString cant_actual_filas = QString::fromStdString(
        "Cantidad de filas actualmente: " + std::to_string(filas));
    QLabel label_cant_actual_filas (cant_actual_filas);
    form_layout.addRow(&label_cant_actual_filas);

    QString cant_actual_columnas = QString::fromStdString(
        "Cantidad de columnas actualmente: " + std::to_string(columnas));
    QLabel label_cant_actual_columnas (cant_actual_columnas);
    form_layout.addRow(&label_cant_actual_columnas);

    QString descripcion_filas ("Filas");
    QSpinBox spinbox_filas (&dialog);
    spinbox_filas.setMinimum(DIMENSION_MINIMA_MAPA);
    spinbox_filas.setMaximum(DIMENSION_MAXIMA_MAPA);
    spinbox_filas.setValue(filas);
    form_layout.addRow(descripcion_filas, &spinbox_filas);

    QString descripcion_columnas ("Columnas");
    QSpinBox spinbox_columnas (&dialog);
    spinbox_columnas.setMinimum(DIMENSION_MINIMA_MAPA);
    spinbox_columnas.setMaximum(DIMENSION_MAXIMA_MAPA);
    spinbox_columnas.setValue(columnas);
    form_layout.addRow(descripcion_columnas, &spinbox_columnas);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Cambiar tamaño del mapa");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        this->mapa.cambiar_tamanio(spinbox_filas.value(), 
            spinbox_columnas.value());

        // actualizo el valor minimo de cantidad de jugadores
        int cantidad_jugadores_agregados = 
            this->mapa.get_cantidad_jugadores_agregados();
        if (cantidad_jugadores_agregados < MINIMO_CANTIDAD_JUGADORES) {
            this->min_cant_jugadores = MINIMO_CANTIDAD_JUGADORES;
        } else {
            this->min_cant_jugadores = cantidad_jugadores_agregados;
        }
    }
}


void Editor::mostrar_dialogo_cantidad_jugadores() {
    QDialog dialog (this);
    QFormLayout form_layout (&dialog);

    QString cant_actual_jugadores = QString::fromStdString(
        "Cantidad de jugadores elegida actualmente: " + 
        std::to_string(this->cant_elegida_jugadores));
    QLabel label_cant_actual_jugadores (cant_actual_jugadores);
    form_layout.addRow(&label_cant_actual_jugadores);

    QString descripcion_cant_jugadores ("Cantidad de jugadores");
    QSpinBox spinbox_jugadores (&dialog);
    spinbox_jugadores.setMinimum(this->min_cant_jugadores);
    spinbox_jugadores.setValue(this->cant_elegida_jugadores);
    form_layout.addRow(descripcion_cant_jugadores, &spinbox_jugadores);

    QDialogButtonBox box_botones (QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
    form_layout.addRow(&box_botones);
    QObject::connect(&box_botones, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setWindowTitle("Cambiar cantidad de jugadores");

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        if (this->cant_elegida_jugadores == spinbox_jugadores.value()) {
            // muestro mensaje de que se cambio la cantidad de jugadores correctamente
            QMessageBox::information(this, "Cantidad de jugadores", 
                "Elegiste la misma cantidad de jugadores!");
        } else {
            this->cant_elegida_jugadores = spinbox_jugadores.value();
    
            // muestro mensaje de que se cambio la cantidad de jugadores correctamente
            QMessageBox::information(this, "Cantidad de jugadores", 
                "Cantidad de jugadores cambiados correctamente.");
        }
    }
}


void Editor::terminar_ejecucion() {
    this->close();
}


Editor::~Editor () {}