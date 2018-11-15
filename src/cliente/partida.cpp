#include "cliente/partida.h"

#include "cliente/servidor.h"

namespace cliente {

Partida::Partida() { 

}

void Partida::pantalla_completa(bool activar) {
    _pantalla_completa = activar;
}

bool Partida::pantalla_completa() const {
    return _pantalla_completa;
}

void Partida::tamanio_ventana(int ancho, int alto) {
    _ancho_ventana = ancho;
    _alto_ventana = alto;
}

int Partida::ancho_ventana() const {
    return _ancho_ventana;
}

int Partida::alto_ventana() const {
    return _alto_ventana;
}

void Partida::vsync(bool activar) {
    _vsync = activar;
}

bool Partida::vsync() const {
    return _vsync;
}

void Partida::musica(bool activar) {
    _musica = activar;
}

bool Partida::musica() const {
    return _musica;
}

void Partida::sonido(bool activar) {
    _sonido = activar;
}

bool Partida::sonido() const {
    return _sonido;
}

void Partida::servidor(Servidor* nuevo_servidor) {
    _servidor = nuevo_servidor;
}

Servidor* Partida::servidor() {
    return _servidor;
}

void Partida::partida_lista(bool esta_lista) {
    _esta_lista = esta_lista;
}

bool Partida::partida_lista() const {
    return _esta_lista && (_servidor != nullptr);
}

} // namespace cliente