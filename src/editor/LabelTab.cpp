#include "LabelTab.h"
#include <iostream>
#include <QPainter>
#define DIMENSION_LABEL 32
using std::string;
using std::vector;

LabelTab::LabelTab(string& id, int tipo, vector<uint32_t>& pos_tiles, 
    QWidget* parent) : QLabel(parent) {
    // fijo el tamaño de Label por DIMENSION_LABEL. 
    this->setFixedSize(DIMENSION_LABEL, DIMENSION_LABEL);
    
    // genero el sprite
    GeneradorSprites generador_sprites;
    this->sprite = generador_sprites.generar_sprite(id, tipo, pos_tiles);
    
    this->setPixmap(this->sprite.imagen);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}


Sprite LabelTab::get_sprite() {
    return this->sprite;
}


void LabelTab::agregar_observador(ObservadorTabs* observador_) {
    this->observador = observador_;
}


void LabelTab::mousePressEvent(QMouseEvent* event) {
    this->observador->en_click_terreno_tab(this->sprite);
}


void LabelTab::set_marco_clickeado() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}


void LabelTab::borrar_marco_clickeado() {
    this->setLineWidth(0);
}
