#include "editor/label_mapa.h"
#include <iostream>
#define TAMANIO_LABEL_MAPA 32
using std::string;

LabelMapa::LabelMapa(Sprite data_sprite, string& position_label, 
    QWidget* parent) : QLabel(parent), sprite(data_sprite), 
    pos_label(position_label) {
    this->setFixedSize(TAMANIO_LABEL_MAPA, TAMANIO_LABEL_MAPA);
    this->setPixmap(sprite.imagen);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}


void LabelMapa::actualizar_sprite(Sprite sprite_nuevo) {
    this->setPixmap(sprite_nuevo.imagen);
    this->sprite = sprite_nuevo;
}


void LabelMapa::actualizar_imagen(QPixmap& nueva_imagen) {
    this->setPixmap(nueva_imagen);
}


Sprite LabelMapa::get_sprite() {
    return this->sprite;
}


void LabelMapa::agregar_observador(ObservadorMapa* observador_) {
    this->observador = observador_;
}


void LabelMapa::mousePressEvent(QMouseEvent* event) {
    this->observador->label_mapa_clickeado(this->pos_label);
}


void LabelMapa::enterEvent(QEvent* event) {
    this->set_marco_mouse_enter();
}


void LabelMapa::leaveEvent(QEvent* event) {
    this->borrar_marco_mouse_enter();
}


void LabelMapa::set_marco_mouse_enter() {
    this->setFrameShape(QFrame::Panel);
    this->setLineWidth(2);
}


void LabelMapa::borrar_marco_mouse_enter() {
    this->setLineWidth(0);
}
