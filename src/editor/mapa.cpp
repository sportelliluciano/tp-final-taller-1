#include "editor/mapa.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include "editor/label_mapa.h"
#include "libs/json.hpp"
#include <sstream>
#include <QMessageBox>
#include <fstream>
#include <QPainter>
#include "editor/manejador_json.h"
#include <QPushButton>
#define DELIM_ID ','
#define TIPO_ROCA 0
using std::string;
using std::stringstream;
using std::getline;
using std::vector;
using std::map;
using nlohmann::json;

Mapa::Mapa(int filas_mapa, int columnas_mapa, QWidget* parent_qwidget) : 
    filas(filas_mapa), columnas(columnas_mapa), parent(parent_qwidget) {
    this->mapa = map<string, LabelMapa*>();
    this->jugadores = map<string, bool>();
}


Mapa::Mapa(QWidget* parent_qwidget) : parent(parent_qwidget) {
    this->mapa = std::map<std::string, LabelMapa*>();
    this->jugadores = map<string, bool>();
}


int Mapa::get_cant_filas() {
    return this->filas;
}


int Mapa::get_cant_columnas() {
    return this->columnas;
}


void Mapa::parsear_json(string& filename_json) {
    // getteo el layout y el widget del Mapa
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    //QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // cosas de parseo del json de terrenos
    std::ifstream entrada(filename_json);
    json mapa_json;
    entrada >> mapa_json;

    vector<vector<uint32_t>> pos_jugadores = mapa_json["jugadores"];
    vector<vector<string>> ids = mapa_json["tipo"];
    this->filas = ids.size();
    
    GeneradorSprites generador_sprites;
    map<string, Sprite> sprites_posibles = generador_sprites.generar_sprites_posibles();
    
    vector<vector<string>>::iterator it_filas_ids = ids.begin();
    this->columnas = (*it_filas_ids).size();

    int fila_actual = 0;
    int columna_actual = 0;
    for (; it_filas_ids != ids.end(); ++it_filas_ids) {
        vector<string>::iterator it_ids = (*it_filas_ids).begin();
        for (; it_ids != (*it_filas_ids).end(); ++it_ids) {
            string id = *it_ids;
            
            string pos_label ("");
            pos_label += std::to_string(fila_actual);
            pos_label += DELIM_ID;
            pos_label += std::to_string(columna_actual);
            
            Sprite sprite;
            map<string, Sprite>::iterator it_sprites = sprites_posibles.find(id);
            if (it_sprites != sprites_posibles.end()) {
                sprite = it_sprites->second;
            }
            sprite.id = id;
            
            LabelMapa* label_mapa = new LabelMapa(sprite, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, fila_actual, columna_actual);
            
            this->mapa.emplace(pos_label, label_mapa);
            
            columna_actual++;
            
            if (columna_actual == this->columnas) {
                columna_actual = 0;
                fila_actual++;
            } 

        }
    }

    // hardcodeo jugador1 como id
    Sprite sprite;
    map<string, Sprite>::iterator it_sprites = sprites_posibles.find("jugador1");
    if (it_sprites != sprites_posibles.end()) {
        sprite = it_sprites->second;
    
    }
    vector<vector<uint32_t>>::iterator it_pos_jugadores = pos_jugadores.begin();
    for (; it_pos_jugadores != pos_jugadores.end(); ++it_pos_jugadores) {
        string id_label ("");
        id_label += std::to_string((*it_pos_jugadores)[0]);
        id_label += ',';
        id_label += std::to_string((*it_pos_jugadores)[1]);
        this->jugadores.emplace(id_label, true);
        map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	    if (it != this->mapa.end()) {
            it->second->actualizar_imagen(sprite.imagen);
        } 
    }
}


void Mapa::inicializar_mapa() {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    //QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // genero sprite inicial
    GeneradorSprites generador_sprites;
    Sprite sprite_inicial = generador_sprites.generar_sprite_inicial();

    for (int i = 0; i < this->filas; ++i) {
        for (int j = 0; j < this->columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i + 1, j + 1);
            
            this->mapa.emplace(pos_label, label_mapa);

            //QPushButton* buttonLeft = new QPushButton(this->parent);
            //buttonLeft->setFixedSize(20, 20);
            //map_layout->addWidget(buttonLeft, i + 1,j +1);
        }
    }
    map_layout->setSpacing(0);

    // Vertical spacers
    //map_layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0, 1, this->filas + 2);
    //map_layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding), this->filas + 1, 0, 1, this->filas + 2);

    // Horizontal spacers
    //map_layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 0, this->columnas, 1);
    //map_layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, this->columnas + 1, this->columnas, 1);

    //scroll_area_mapa->setLayout(map_layout);
}


int Mapa::get_tipo_by_id(string& id_label_mapa) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label_mapa);
	if (it != this->mapa.end()) {
        return it->second->get_sprite().tipo;
    }
}


bool Mapa::es_valido_agregar_jugador(string& id_label_mapa, 
    int cantidad_jugadores) {
    // Verifico que se agregue el jugador sobre Roca.
    if (this->get_tipo_by_id(id_label_mapa) == TIPO_ROCA) {
         // me fijo si faltan ubicar jugadores o ya fueron todos ubicados
        if (this->get_cantidad_jugadores_agregados() < cantidad_jugadores) {
            // me fijo si ya hay un jugador en esa posicion del Mapa
            if (this->jugadores.find(id_label_mapa) == this->jugadores.end()) {
                return true;
            } else {
                QMessageBox::critical(0, "Error al agregar un jugador", 
                    "Ya agregaste un jugador en esa posicion");
                return false;
            }          
        } else {
            QMessageBox::critical(0, "Error al agregar un jugador", 
                "Ya alcanzaste el maximo de jugadores posibles");
            return false;
        }    
    } else {
        QMessageBox::critical(0, "Error al agregar un jugador", 
            "Solo se puede agregar jugadores sobre roca");
        return false;
    }
}


void Mapa::generar_json(string& nombre_archivo) {
    ManejadorJson manejador_json;
    manejador_json.generar_json(nombre_archivo, this->filas, this->columnas,
        this->mapa, this->jugadores);
}


void Mapa::agregar_observador(Observador* observer) {
    this->observador = observer;
}


void Mapa::actualizar_data(string& id_label, Sprite sprite_nuevo) {
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        // me fijo si es el caso especial en que reemplazo a un jugador por otra
        // cosa que no es jugador.
        map<string, bool>::iterator it_jugadores = this->jugadores.find(id_label);
        if (it_jugadores != this->jugadores.end()) {
            this->jugadores.erase(it_jugadores);
        }
        it->second->actualizar_sprite(sprite_nuevo);
    }
}

void Mapa::agregar_jugador(string& id_label, Sprite nuevo_sprite) {
    // agrego jugador al mapa jugadores.
    this->jugadores.emplace(id_label, true);

    // actualizo la imagen del LabelMapa correspondiente.
    map<string, LabelMapa*>::iterator it = this->mapa.find(id_label);
	if (it != this->mapa.end()) {
        it->second->actualizar_imagen(nuevo_sprite.imagen);
    }
}


void Mapa::label_mapa_clickeado(std::string& id_label_mapa) {
    this->observador->en_notificacion(id_label_mapa);
}


int Mapa::get_cantidad_jugadores_agregados() {
    return this->jugadores.size();
}


void Mapa::cambiar_tamanio(int nueva_cant_filas, int nueva_cant_columnas) {
    if (nueva_cant_columnas == this->columnas && nueva_cant_filas == this->filas) {
        return;
    }

    if (nueva_cant_columnas >= this->columnas && nueva_cant_filas >= this->filas) {
        agrandar_mapa(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas <= this->columnas && nueva_cant_filas <= this->filas) {
        achicar_mapa(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas < this->columnas && nueva_cant_filas > this->filas) {
        sacar_columnas_agregar_filas(nueva_cant_filas, nueva_cant_columnas);
    } else if (nueva_cant_columnas > this->columnas && nueva_cant_filas < this->filas) {
        sacar_filas_agregar_columnas(nueva_cant_filas, nueva_cant_columnas);
    }
}

void Mapa::agrandar_mapa(int nueva_cant_filas, int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");
     
    // genero sprite inicial o default
    GeneradorSprites generador_sprites;
    Sprite sprite_inicial = generador_sprites.generar_sprite_inicial();
    
    for (int i = 0; i < this->filas; ++i) {
        for (int j = this->columnas; j < nueva_cant_columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    for (int j = 0; j < nueva_cant_columnas; ++j) {
        for (int i = this->filas; i < nueva_cant_filas; ++i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }


    this->filas = nueva_cant_filas;
    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::achicar_mapa(int nueva_cant_filas, int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");
    
    for (int i = 0; i < nueva_cant_filas; ++i) {
        for (int j = (this->columnas - 1); j >= nueva_cant_columnas; --j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
                this->mapa.erase(it);
            }

            // me fijo si habia un jugador en esa posicion
            map<string, bool>::iterator it_jugadores = this->jugadores.find(pos_label);
            if (it_jugadores != this->jugadores.end()) {
                this->jugadores.erase(it_jugadores);
            }
        }
    }

    for (int j = 0; j < this->columnas; ++j) {
        for (int i = (this->filas - 1); i >= nueva_cant_filas; --i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
                this->mapa.erase(it);
            }

            // me fijo si habia un jugador en esa posicion
            map<string, bool>::iterator it_jugadores = this->jugadores.find(pos_label);
            if (it_jugadores != this->jugadores.end()) {
                this->jugadores.erase(it_jugadores);
            }
        }
    }


    this->filas = nueva_cant_filas;
    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::sacar_columnas_agregar_filas(int nueva_cant_filas, 
    int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // genero sprite inicial o default
    GeneradorSprites generador_sprites;
    Sprite sprite_inicial = generador_sprites.generar_sprite_inicial();
    
    // saco columnas
    for (int i = 0; i < nueva_cant_filas; ++i) {
        for (int j = (this->columnas - 1); j >= nueva_cant_columnas; --j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
                this->mapa.erase(it);
            }

            // me fijo si habia un jugador en esa posicion
            map<string, bool>::iterator it_jugadores = this->jugadores.find(pos_label);
            if (it_jugadores != this->jugadores.end()) {
                this->jugadores.erase(it_jugadores);
            }
        }
    }

    this->columnas = nueva_cant_columnas;

    // agrego filas
    for (int j = 0; j < nueva_cant_columnas; ++j) {
        for (int i = this->filas; i < nueva_cant_filas; ++i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    this->filas = nueva_cant_filas;
    scroll_area_mapa->setLayout(map_layout);
}

void Mapa::sacar_filas_agregar_columnas(int nueva_cant_filas, 
    int nueva_cant_columnas) {
    // getteo el layout y el widget del Mapa-
    QGridLayout* map_layout = this->parent->findChild<QGridLayout*>("mapLayout");
    QWidget* scroll_area_mapa = this->parent->findChild<QWidget*>("scrollArea_widget_mapa");

    // genero sprite inicial
    GeneradorSprites generador_sprites;
    Sprite sprite_inicial = generador_sprites.generar_sprite_inicial();

    // saco filas
    for (int j = 0; j < this->columnas; ++j) {
        for (int i = (this->filas - 1); i >= nueva_cant_filas; --i) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            map<string, LabelMapa*>::iterator it = this->mapa.find(pos_label);
            if (it != this->mapa.end()) {
                map_layout->removeWidget(it->second);
                delete it->second;
                this->mapa.erase(it);
            }

            // me fijo si habia un jugador en esa posicion
            map<string, bool>::iterator it_jugadores = this->jugadores.find(pos_label);
            if (it_jugadores != this->jugadores.end()) {
                this->jugadores.erase(it_jugadores);
            }
        }
    }

    this->filas = nueva_cant_filas;

    // agrego columnas
    for (int i = 0; i < this->filas; ++i) {
        for (int j = this->columnas; j < nueva_cant_columnas; ++j) {
            string pos_label ("");
            pos_label += std::to_string(i);
            pos_label += DELIM_ID;
            pos_label += std::to_string(j);
            
            LabelMapa* label_mapa = new LabelMapa(sprite_inicial, pos_label, 
                this->parent);
            
            label_mapa->agregar_observador(this);
            
            map_layout->addWidget(label_mapa, i, j);
            
            this->mapa.emplace(pos_label, label_mapa);
        }
    }

    this->columnas = nueva_cant_columnas;
    scroll_area_mapa->setLayout(map_layout);
}


void Mapa::limpiar_mapa() {
    map<string, LabelMapa*>::iterator it = this->mapa.begin();
    for (; it != this->mapa.end(); ++it) {
        delete it->second;
        this->mapa.erase(it);
    }

    this->jugadores.clear();
}

Mapa::~Mapa() {
    map<string, LabelMapa*>::iterator it = this->mapa.begin();
    for (; it != this->mapa.end(); ++it) {
        delete it->second;
        this->mapa.erase(it);
    }
}
        