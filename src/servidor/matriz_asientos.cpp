#include "matriz_asientos.h"

#include <sstream>
#include <string>
#include <vector>

#include "error_valor.h"

MatrizAsientos::MatrizAsientos(uint32_t cantidad_filas, 
    uint32_t cantidad_columnas)
    : n_asientos(cantidad_filas * cantidad_columnas),
      asientos(std::vector<bool>(n_asientos)) {
    for (unsigned int i=0; i<asientos.size(); i++) {
        asientos[i] = false;
    }
    filas = cantidad_filas;
    columnas = cantidad_columnas;
}

MatrizAsientos::MatrizAsientos(const std::vector<uint8_t>& serializado) {
    filas = serializado[0];
    columnas = serializado[1];
    n_asientos = filas * columnas;
    asientos = std::vector<bool>(n_asientos);
    
    uint8_t indice = 1;
    uint8_t offset = 8;
    for (uint8_t i=0; i<n_asientos; i++) {
        if (offset == 8) {
            indice++;
            offset = 0;
        }

        asientos[i] = serializado[indice] & (1 << offset);
        offset++;
    }
}

MatrizAsientos::MatrizAsientos(const MatrizAsientos& otro)
    : n_asientos(otro.n_asientos), asientos(std::vector<bool>(n_asientos))
{
    for (unsigned int i=0; i<asientos.size(); i++) {
        asientos[i] = otro.asientos[i];
    }
    filas = otro.filas;
    columnas = otro.columnas;
}

bool MatrizAsientos::esta_ocupado(uint32_t fila, uint32_t columna) const {
    if ((fila >= filas) || (columna >= columnas))
        throw ErrorValor("El asiento no existe");
    
    size_t indice = (fila * columnas) + columna;
    return asientos[indice];
}

bool MatrizAsientos::reservar(uint32_t fila, uint32_t columna) {
    if ((fila >= filas) || (columna >= columnas))
        throw ErrorValor("El asiento no existe");
    
    size_t indice = (fila * columnas) + columna;
    if (asientos[indice])
        return false;

    asientos[indice] = true;
    return true;
}

bool MatrizAsientos::esta_agotada() const {
    for (uint32_t i=0; i<asientos.size(); i++) {
        if (!asientos[i])
            return false;
    }

    return true;
}

const std::string MatrizAsientos::como_cadena() const {
    std::stringstream str;
    for (uint32_t i=0; i<columnas; i++) {
        str << "\t" << (i + 1);
    }
    str << "\n";
    for (uint32_t f=0; f<filas; f++) {
        str << (char)('A' + f);
        for (uint32_t c=0; c<columnas; c++) {
            str << "\t" << (esta_ocupado(f, c) ? "X":"O");
        }
        str << "\n";
    }
    return str.str();
}

const std::vector<uint8_t> MatrizAsientos::serializar() const {
    std::vector<uint8_t> resultado((n_asientos / 8) + 3);
    resultado[0] = (uint8_t) filas;
    resultado[1] = (uint8_t) columnas;
    uint8_t indice = 1;
    uint8_t offset = 8;
    for (uint8_t i=0; i<asientos.size(); i++) {
        if (offset == 8) {
            indice++;
            offset = 0;
            resultado[indice] = 0;
        }

        if (asientos[i])
            resultado[indice] |= (uint8_t)(1 << offset);

        offset++;
    }

    return std::move(resultado);
}

