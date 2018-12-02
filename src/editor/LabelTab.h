#ifndef LABEL_TAB_H
#define LABEL_TAB_H

#include <QWidget>
#include <QLabel>
#include <string>
#include <vector>
#include "ObservadorTabs.h"
#include "GeneradorSprites.h"

class LabelTab : public QLabel {
    public:
        /**
         * \brief Constructor de LabelTab.
         * 
         * Constructor de LabelTab que recibe como parametro el id, el tipo y 
         * la posicion de los tiles de este LabelTab en particular.
         */
        LabelTab(std::string& id, int tipo, std::vector<uint32_t>& pos_tiles, 
            QWidget* parent = 0);

        /**
         * \brief Getter sprite del LabelTab.
         * 
         * Devuelvo el sprite del LabelTab.
         */
        Sprite get_sprite();
	    
        /**
         * \brief Agrego observador del LabelTab.
         * 
         * Agrego un observador del LabelTab (Tabs).
         */
        void agregar_observador(ObservadorTabs* observador);

        /**
         * \brief Agrego el marco clickeado.
         * 
         * Agrego el marco clickeado.
         */
        void set_marco_clickeado();

        /**
         * \brief Borro el marco clickeado.
         * 
         * Borro el marco clickeado.
         */
        void borrar_marco_clickeado();
        
        /**
         * \brief Destructor de LabelTab.
         * 
         * Destructor de LabelTab.
         */
        ~LabelTab() {};
 
    protected:
        /**
         * \brief Atrapo mouse clicks sobre el LabelTab.
         * 
         * Le aviso al observador que fue clickeado.
         */
	    void mousePressEvent(QMouseEvent * event);

    private:
        Sprite sprite;
        ObservadorTabs* observador;
};

#endif // LABEL_TAB_H
