#include "conexion/conexion.h"

#include <cstdlib>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "libs/json.hpp"

#include "conexion/error_socket.h"
#include "conexion/error_conexion.h"
#include "conexion/socket_conexion.h"

namespace conexion {

Conexion::Conexion() : esta_conectado_rd(false), esta_conectado_wr(false) { }

Conexion::Conexion(SocketConexion& socket) 
    : conexion(std::move(socket)), esta_conectado_rd(true), 
      esta_conectado_wr(true)
{ }

Conexion::Conexion(const std::string& direccion, const std::string& servicio) 
    : conexion(SocketConexion(direccion, servicio))
{ }

Conexion::Conexion(Conexion&& otro) {
    esta_conectado_rd = otro.esta_conectado_rd;
    esta_conectado_wr = otro.esta_conectado_wr;
    conexion = std::move(otro.conexion);

    otro.esta_conectado_rd = otro.esta_conectado_wr = false;
}

Conexion& Conexion::operator=(Conexion&& otro) {
    if (this == &otro)
        return *this;

    esta_conectado_rd = otro.esta_conectado_rd;
    esta_conectado_wr = otro.esta_conectado_wr;
    conexion = std::move(otro.conexion);
    
    otro.esta_conectado_rd = otro.esta_conectado_wr = false;
    return *this;
}

nlohmann::json Conexion::recibir_json() {
    std::stringstream resultado;
    uint8_t byte_leido;
    while ((byte_leido = leer_uint8()) != '\n')
        resultado << (char) byte_leido;

    std::cout << "\x1b[32m>> \x1b[0m" << resultado.str() << std::endl;
    return nlohmann::json::parse(resultado.str());
}

void Conexion::enviar_json(const nlohmann::json& json_data) {
    std::string json_str = json_data.dump() + "\n";
    enviar_bytes((uint8_t*)json_str.c_str(), json_str.length()); // + \0
    std::cout << "\x1b[31m<< \x1b[0m" << json_data.dump() << std::endl;
}

bool Conexion::esta_conectada() const {
    return esta_conectado_rd && esta_conectado_wr;
}

void Conexion::cerrar(bool ignorar_errores) {
    if (esta_conectado_rd || esta_conectado_wr) {
        conexion.shutdown(esta_conectado_rd, esta_conectado_wr, 
            ignorar_errores);
        conexion.close(ignorar_errores);
        esta_conectado_rd = esta_conectado_wr = false;
    }
}

Conexion::~Conexion() {
    // Prevenir excepciones en el destructor.
    cerrar(true);
}

size_t Conexion::recibir_bytes(uint8_t *buffer_, size_t cantidad) {
    size_t bytes_totales = 0;
    unsigned char *buffer = buffer_;
    
    while (bytes_totales < cantidad) {
        ssize_t bytes_recibidos = conexion.recv(buffer + bytes_totales, 
            cantidad - bytes_totales);

        if (bytes_recibidos == 0) {
            esta_conectado_rd = false;
            return bytes_totales;
        }

        bytes_totales += bytes_recibidos;
    }

    return bytes_totales;
}

size_t Conexion::enviar_bytes(const uint8_t *buffer_, size_t cantidad) {
    size_t bytes_totales = 0;
    const unsigned char *buffer = buffer_;

    while (bytes_totales < cantidad) {
        ssize_t bytes_enviados = conexion.send(buffer + bytes_totales, 
            cantidad - bytes_totales);
        
        if (bytes_enviados == 0) {
            esta_conectado_wr = false;
            return bytes_totales;
        }

        bytes_totales += bytes_enviados;
    }

    return bytes_totales;
}

uint8_t Conexion::leer_uint8() {
    uint8_t resultado;
    if (recibir_bytes(&resultado, 1) != 1)
        throw ErrorConexion();
    return resultado;
}

} // namespace conexion
