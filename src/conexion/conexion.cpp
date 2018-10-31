#include "conexion/conexion.h"

#include <cstdlib>

#include <sstream>
#include <string>
#include <vector>

#include "libs/json.hpp"

#include "conexion/error_socket.h"
#include "conexion/error_conexion.h"
#include "conexion/evento.h"
#include "conexion/factory_evento.h"
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

Evento* Conexion::recibir_evento() {
    std::stringstream resultado;
    uint8_t byte_leido;
    while ((byte_leido = leer_uint8()) != '\n')
        resultado << (char) byte_leido;

    return FactoryEvento::crear_desde_json(
            nlohmann::json::parse(resultado.str())
        );
}

void Conexion::enviar_evento(const Evento& evento) {
    std::string json_str = evento.serializar().dump();
    enviar_bytes((uint8_t*)json_str.c_str(), json_str.length() + 1); // + \0
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
