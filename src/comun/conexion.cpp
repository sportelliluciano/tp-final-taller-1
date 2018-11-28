#include "comun/conexion.h"

#include <cstdlib>

#include <iostream>
#include <string>

#include "libs/json.hpp"

#include "comun/error_socket.h"
#include "comun/error_conexion.h"
#include "comun/socket_conexion.h"

// Descomentar esta línea para ver los paquetes de red enviados
//#define DEPURACION_RED 1

/**
 * La conexión sólo permite enviar paquetes de un tamaño máximo de 2MB.
 * Este valor debería ser más que suficiente para todos los propósitos de
 * este proyecto (Dune Remake).
 * 
 * El tamaño máximo está impuesto para evitar que un paquete demasiado grande
 * termine ocupando toda la memoria del cliente/servidor.
 * 
 */
#define TAMANIO_MAXIMO_PAQUETE (2048 * 1024)



Conexion::Conexion(SocketConexion& socket) 
    : conexion(std::move(socket)),
      buffer_recepcion(new uint8_t[TAMANIO_MAXIMO_PAQUETE + 1])
{ }

Conexion::Conexion(const std::string& direccion, const std::string& servicio) 
    : conexion(SocketConexion(direccion, servicio)),
      buffer_recepcion(new uint8_t[TAMANIO_MAXIMO_PAQUETE + 1])
{ }

Conexion::Conexion(Conexion&& otro) 
    : conexion(std::move(otro.conexion)), 
      buffer_recepcion(std::move(otro.buffer_recepcion))
{ }

Conexion& Conexion::operator=(Conexion&& otro) {
    if (this == &otro)
        return *this;

    conexion = std::move(otro.conexion);
    buffer_recepcion = std::move(otro.buffer_recepcion);
    return *this;
}

/**
 * Protocolo de comunicación:
 *  Se envía el largo de la serialización en 4 bytes big endian y luego la 
 *  serialización en formato JSON. La misma debe estar codificada en UTF-8
 */
nlohmann::json Conexion::recibir_json() {
    uint32_t tamanio = 0;
    if (conexion.recibir_bytes((uint8_t*)&tamanio, sizeof(tamanio)) 
         != sizeof(tamanio)) 
    {
        throw ErrorConexion("Se cerró la conexión");
    }

    tamanio = ntohl(tamanio);

    if (tamanio >= TAMANIO_MAXIMO_PAQUETE) {
        throw ErrorConexion("Se recibió un tamaño de paquete inválido");
    }

    if (conexion.recibir_bytes(buffer_recepcion.get(), tamanio) != tamanio) {
        throw ErrorConexion("Se cerró la conexión durante la recepción de "
            "un paquete");
    }

    buffer_recepcion.get()[tamanio] = 0;

    nlohmann::json resultado;
    
    try {
        resultado = nlohmann::json::parse(buffer_recepcion.get());
    } catch (const std::exception& e) {
        throw ErrorConexion("Se recibió un paquete inválido");
    }
#ifdef DEPURACION_RED
    if (tamanio > 1000) {
        std::cout << "\x1b[32m>> \x1b[0m {JSON de " << tamanio << " bytes}" 
                  << std::endl;
    } else {
        std::cout << "\x1b[32m>> \x1b[0m" << resultado.dump() << std::endl;
    }
#endif
    return resultado;
}

void Conexion::enviar_json(const nlohmann::json& json_data) {
    const std::string& s = json_data.dump();
    if (s.length() >= TAMANIO_MAXIMO_PAQUETE) {
        throw ErrorConexion("El paquete a enviar es demasiado grande");
    }
    const uint8_t* data_utf8 = (uint8_t*)(s.c_str());
    uint32_t tamanio = (uint32_t) s.length();
    uint32_t tamanio_nbo = htonl(tamanio);

    if (conexion.enviar_bytes((const uint8_t*)&tamanio_nbo, sizeof(tamanio)) 
        != sizeof(tamanio)) 
    {
        throw ErrorConexion("Se cerró la conexión");
    }

    if (conexion.enviar_bytes(data_utf8, tamanio) != tamanio) {
        throw ErrorConexion("Se cerró la conexión en medio del envío");
    }
#ifdef DEPURACION_RED    
    if (tamanio > 1000) {
        std::cout << "\x1b[31m<< \x1b[0m {JSON de " << tamanio << " bytes}"
                  << std::endl;
    } else {
        std::cout << "\x1b[31m<< \x1b[0m" << json_data.dump() << std::endl;
    }
#endif
}

bool Conexion::esta_conectada() const {
    return conexion.puede_enviar() && conexion.puede_recibir();
}

void Conexion::cerrar(bool ignorar_errores) {
    conexion.shutdown(true, true, ignorar_errores);
    conexion.close(ignorar_errores);
}

Conexion::~Conexion() { }


