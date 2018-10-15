#include "conexion.h"

#include <cstdlib>

#include <sstream>
#include <string>
#include <vector>

#include "socket_conexion.h"
#include "error_socket.h"
#include "error_conexion.h"

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

bool Conexion::recibir_bool() {
    uint8_t tipo = leer_uint8();
    if (tipo == TIPO_BOOL_TRUE)
        return true;
    if (tipo == TIPO_BOOL_FALSE)
        return false;
    
    throw ErrorConexion();
}

std::vector<uint8_t> Conexion::recibir_objeto() {
    verificar_tipo(TIPO_OBJETO);
    
    uint32_t largo = recibir_entero();
    
    std::vector<uint8_t> serializado(largo);
    for (uint32_t i=0; i<largo; i++) {
        serializado[i] = leer_uint8();
    }
    return std::move(serializado);
}

uint32_t Conexion::recibir_entero() {
    verificar_tipo(TIPO_ENTERO);
    uint32_t valor;
    recibir_bytes((uint8_t*)&valor, sizeof(uint32_t));
    return ntohl(valor);
}

std::string Conexion::recibir_string() {
    verificar_tipo(TIPO_STRING);
    
    std::stringstream resultado;
    uint8_t byte_leido;
    while ((byte_leido = leer_uint8()) != '\0')
        resultado << (char) byte_leido;

    return std::move(resultado.str());;
}

comando_t Conexion::recibir_comando() {
    verificar_tipo(TIPO_COMANDO);
    
    return (comando_t) leer_uint8();
}

void Conexion::enviar_bool(bool valor) {
    uint8_t tipo = valor ? TIPO_BOOL_TRUE : TIPO_BOOL_FALSE;
    enviar_bytes(&tipo, 1);
}

void Conexion::enviar_objeto(const std::vector<uint8_t>& serializado) {
    uint8_t tipo = TIPO_OBJETO;
    enviar_bytes(&tipo, 1);
    enviar_entero((uint32_t)serializado.size());
    for (uint32_t i=0; i<serializado.size(); i++) {
        uint8_t dato = serializado[i];
        enviar_bytes(&dato, 1);
    }
}

void Conexion::enviar_entero(uint32_t valor) {
    uint8_t tipo = TIPO_ENTERO;
    enviar_bytes(&tipo, 1);
    uint32_t valor_nbo = htonl(valor);
    enviar_bytes((uint8_t*)&valor_nbo, sizeof(uint32_t));
}

void Conexion::enviar_string(const std::string& valor) {
    uint8_t tipo = TIPO_STRING;
    enviar_bytes(&tipo, 1);
            
    enviar_bytes((const uint8_t*)valor.c_str(), valor.length() + 1);
}

void Conexion::enviar_comando(comando_t comando) {
    uint8_t datos[2] = { TIPO_COMANDO, comando };
    enviar_bytes(datos, 2);
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

void Conexion::verificar_tipo(tipo_dato_t tipo_esperado) {
    uint8_t tipo = leer_uint8();
    if (tipo != tipo_esperado)
        throw ErrorConexion();
}
