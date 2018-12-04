#include "cliente/red/servidor_protegido.h"

#include <mutex>
#include <string>
#include <vector>

#include "libs/json.hpp"

#include "cliente/red/servidor.h"

namespace cliente {

ServidorProtegido::ServidorProtegido() { }

void ServidorProtegido::set_servidor(Servidor* servidor_a_proteger) {
    servidor = servidor_a_proteger;
}

void ServidorProtegido::set_casa(const std::string& casa) {
    std::lock_guard<std::mutex> l(m_servidor);
    servidor->set_casa(casa);
}

void ServidorProtegido::set_nombre(const std::string& nombre) {
    std::lock_guard<std::mutex> l(m_servidor);
    servidor->set_nombre(nombre);
}

void ServidorProtegido::iniciar_juego() {
    std::lock_guard<std::mutex> l(m_servidor);
    servidor->iniciar_juego();
}

std::vector<nlohmann::json> ServidorProtegido::obtener_jugadores_sala(
    const std::string& sala) 
{
    return servidor->obtener_jugadores_en(sala);
}


std::vector<std::string> ServidorProtegido::obtener_nombres_salas() {
    std::lock_guard<std::mutex> l(m_servidor);
    return servidor->obtener_salas();
}

std::vector<std::string> ServidorProtegido::obtener_nombres_mapas() {
    std::lock_guard<std::mutex> l(m_servidor);
    return servidor->obtener_mapas();
}

bool ServidorProtegido::unirse_a_sala(const std::string& sala) {
    std::lock_guard<std::mutex> l(m_servidor);
    return servidor->unirse_a_sala(sala);
}

bool ServidorProtegido::dejar_sala() {
    std::lock_guard<std::mutex> l(m_servidor);
    return servidor->dejar_sala();
}

bool ServidorProtegido::crear_sala(const std::string& nombre, const std::string& mapa) {
    std::lock_guard<std::mutex> l(m_servidor);
    return servidor->crear_sala(nombre, mapa);
}

} // namespace cliente
