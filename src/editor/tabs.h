#ifndef TABS_H
#define TABS_H

#include <string>
#include <map>
#include <vector>
#include "editor/label_tab.h"
#include "editor/observador_tabs.h"
#include "editor/sprite.h"

class Tabs : public ObservadorTabs {
    public:
        /**
         * \brief Constructor Tabs.
         * 
         * Es llamado por el constructor de Editor, por lo tanto se separa la creacion
         * de la inicializacion de Tabs (el Editor debe hacer setupUi para poder 
         * utilizar los elementos graficos de Qt como QGridLayout).
         */
        explicit Tabs(QWidget* parent = 0);

        /**
         * \brief Inicializador Tabs.
         * 
         * Llena la pestaña sprites.
         */
        void inicializar_tabs();

        /**
         * \brief Getter sprite clickeado.
         * 
         * Devuelvo el sprite clickeado, puede estar vacio su id ("").
         */
        Sprite get_sprite_clickeado();

        /**
         * \brief Metodo virtual de la interfaz Observador implementada por Tabs.
         * 
         * Metodo virtual que es llamado cuando un label de una pestaña (LabelTab)
         * es clickeado. Recibe por parametro el id del Label.
         */
        virtual void en_click_terreno_tab(Sprite sprite_clickeado);

        /**
         * \brief Destructor de Tabs.
         * 
         * Libero los Label del heap (fueron tomados los recursos en 
         *  Tabs::inicializar_tabs)
         */
        ~Tabs();

    private:
        QWidget* parent;
        QPixmap imagen_terrenos;
        std::map<std::string, LabelTab*> tabs_terrenos;
        Sprite sprite_clickeado;
};

#endif // TABS_H