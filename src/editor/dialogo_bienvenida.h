#ifndef DIALOGO_BIENVENIDA_H
#define DIALOGO_BIENVENIDA_H

#include <QDialog>
#include <QFormLayout>
#include "editor/editor.h"

class DialogoBienvenida : public QDialog {
    public:
        /**
         * \brief Constructor de DialogoBienvenida.
         * 
         * Constructor de DialogoBienvenida que es utilizado como menu principal al 
         * iniciar el editor.
         */
        explicit DialogoBienvenida(QWidget *parent = 0);

         /**
         * \brief Muestro dialogo crear mapa.
         * 
         * Es llamada al tocar el boton de crear mapa (vease main.cpp). Muestra el 
         * dialogo con las configuraciones posibles para un mapa nuevo 
         * (tamaño y cantidad de jugadores).
         */
        void mostrar_dialogo_crear_mapa();

        /**
         * \brief Muestro dialogo cargar mapa.
         * 
         * Es llamada al tocar el boton de cargar mapa (vease main.cpp). Muestra el 
         * dialogo para elegir al mapa previamente creado y almacenado en la pc.
         */
        void mostrar_dialogo_cargar_mapa();

        /**
         * \brief Destructor DialogoBienvenida.
         * 
         * Verifica si fue creado el editor en el heap, en caso afirmativo, lo libero.
         * Siempre liberp el titulo y los botones.
         */
        ~DialogoBienvenida(); 

    private:
        Editor* editor;
        QFormLayout* form_layout;
        QLabel* titulo;
        QPushButton* boton_crear_mapa;
        QPushButton* boton_cargar_mapa;
        bool editor_fue_creado;
};

#endif // DIALOGO_BIENVENIDA_H