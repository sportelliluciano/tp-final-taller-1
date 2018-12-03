#ifndef LABEL_MAPA_H
#define LABEL_MAPA_H

#include <QWidget>
#include <QLabel>
#include <string>
#include "editor/observador_mapa.h"
#include "editor/sprite.h"

class LabelMapa : public QLabel {
    public:
        /**
         * \brief Constructor de LabelMapa.
         * 
         * Constructor de LabelMapa que recibe como parametro el Sprite 
         * correspondiente, la pos_label dentro del mapa y el widget padre.
         */
        LabelMapa(Sprite sprite, std::string& pos_label, QWidget* parent = 0);

        /**
         * \brief Agrego observador del LabelMapa.
         * 
         * Agrego un observador del LabelMapa (Mapa).
         */
        void agregar_observador(ObservadorMapa* observador);
        
        /**
         * \brief Actualizar sprite del LabelMapa.
         * 
         * Actualizo el sprite del LabelMapa.
         */
        void actualizar_sprite(Sprite nuevo_sprite);

        /**
         * \brief Actualizar imagen del LabelMapa.
         * 
         * Actualizo la imagen del LabelMapa.
         */
        void actualizar_imagen(QPixmap& nueva_imagen);
        
        /**
         * \brief Agrego el marco clickeado.
         * 
         * Agrego el marco clickeado.
         */
        void set_marco_mouse_enter();

        /**
         * \brief Borro el marco clickeado.
         * 
         * Borro el marco clickeado.
         */
        void borrar_marco_mouse_enter();

        /**
         * \brief Getter Sprite del LabelMapa.
         * 
         * Devuelvo el sprite del LabelMapa.
         */
        Sprite get_sprite();

        /**
         * \brief Destructor de LabelMapa.
         * 
         * Destructor de LabelMapa.
         */
        ~LabelMapa() {};
 
    protected:
        /**
         * \brief Atrapo mouse clicks sobre el LabelMapa.
         * 
         * Le aviso al observador que fue clickeado. 
         */
	    void mousePressEvent(QMouseEvent * event);

        /**
         * \brief Atrapo entradas del mouse sobre el LabelMapa.
         * 
         * Le aviso al observador que el mouse entro al LabelMapa. 
         */
        void enterEvent(QEvent* event);

        /**
         * \brief Atrapo salidas del mouse del LabelMapa.
         * 
         * Le aviso al observador que el mouse salio del LabelMapa. 
         */
        void leaveEvent(QEvent* event);

    private:
        Sprite sprite;
        std::string pos_label;
        ObservadorMapa* observador;
};

#endif // LABEL_H