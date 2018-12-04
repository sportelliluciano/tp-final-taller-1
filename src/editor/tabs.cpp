#include "editor/tabs.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "editor/label_tab.h"
#include "libs/json.hpp"
#include <fstream>
#include <vector>
#include "editor/config.h"
using std::string;
using std::map;
using nlohmann::json;
using std::vector;

Tabs::Tabs(QWidget* parent_qwidget) : parent(parent_qwidget) {
    this->tabs_terrenos = map<string, LabelTab*>();
    Sprite sprite;
    sprite.id = "";
    this->sprite_clickeado = sprite;
}


void Tabs::inicializar_tabs() {
    // getteo el layout y el widget de Terrenos
    QVBoxLayout* terrenos_layout = this->parent->findChild<QVBoxLayout*>("verticalLayout_terrenos");
    QWidget* scroll_area_terrenos = this->parent->findChild<QWidget*>("scrollArea_widget_terrenos");

    std::ifstream entrada(RUTA_TERRENOS_JSON);

    json terrenos_json;

    entrada >> terrenos_json;

    auto it = terrenos_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for (; it != terrenos_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        auto it_sprites = elem["sprites"].begin();
        for (int i = 0; it_sprites != elem["sprites"].end(); ++it_sprites) {
            json tile = *it;

            string id = tile["sprites"][i]["id"];
            vector<uint32_t> pos_tiles = tile["sprites"][i]["pos_tiles"];
            
            LabelTab* label = new LabelTab(id, elem["tipo"], pos_tiles, 
                this->parent);
                       
            label->agregar_observador(this);

            // agrego y acomodo el Label
            terrenos_layout->addWidget(label);
            terrenos_layout->setAlignment(label, Qt::AlignHCenter);
        
            this->tabs_terrenos.emplace(tile["sprites"][i]["id"], label);
            
            i++;
        }
    }

    // asocio el QVBoxLayout con su respectiva scroll area.
    scroll_area_terrenos->setLayout(terrenos_layout);
}

Sprite Tabs::get_sprite_clickeado() {
    return this->sprite_clickeado;
}


void Tabs::en_click_terreno_tab(Sprite sprite) {
    // me fijo que tenga almacenado al Label en tabs_terrenos
    map<string, LabelTab*>::iterator it = 
        this->tabs_terrenos.find(sprite.id);
	if (it != this->tabs_terrenos.end()) {
        // me fijo si el id coincide con el id del label clickeado actualmente.
        if (this->sprite_clickeado.id == sprite.id) {
            it->second->borrar_marco_clickeado();
            this->sprite_clickeado.id = "";
        } else {
            // no coinciedo => le agrego el marco de clickeado y borro el marco
            // del clickeado anteriormente.
            it->second->set_marco_clickeado();
            map<string, LabelTab*>::iterator it2 = 
                this->tabs_terrenos.find(this->sprite_clickeado.id);
	        if (it2 != this->tabs_terrenos.end()) {
                it2->second->borrar_marco_clickeado();
            }
            this->sprite_clickeado = sprite;
        }
    }
}

Tabs::~Tabs() {
    map<string, LabelTab*>::iterator it = this->tabs_terrenos.begin();
    for (; it != this->tabs_terrenos.end(); ++it) {
        delete it->second;
    }
}
