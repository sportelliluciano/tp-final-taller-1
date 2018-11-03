#ifndef _MATRIZ_ASIENTOS_H_
#define _MATRIZ_ASIENTOS_H_

#include <sstream>
#include <string>
#include <vector>

#include "error_valor.h"

/**
 * Matriz de disponibilidad de asientos de una función.
 * 
 * Este objeto es serializable.
 */
class MatrizAsientos {
public:
    /**
     * Constructor.
     * Crea una matriz de disponibilidad inicialmente vacía a partir
     * de la cantidad de filas y columnas de la misma.
     */
    MatrizAsientos(uint32_t cantidad_filas, uint32_t cantidad_columnas);

    /**
     * Constructor por deserialización.
     * 
     * Construye una matriz de asientos a partir de una secuencia de bytes.
     */
    explicit MatrizAsientos(const std::vector<uint8_t>& serializado);

    /**
     * Constructor por copia 
     */
    MatrizAsientos(const MatrizAsientos& otro);

    /**
     * Devuelve true si el asiento está ocupado.
     * 
     * Lanza ErrorValor si el asiento no existe.
     */
    bool esta_ocupado(uint32_t fila, uint32_t columna) const;

    /**
     * Devuelve true si el asiento está ocupado.
     * 
     * Lanza ErrorValor si el asiento no existe.
     */
    bool reservar(uint32_t fila, uint32_t columna);

    /**
     * Devuelve true si no quedan asientos disponibles.
     */
    bool esta_agotada() const;

    /**
     * Devuelve una representación en cadena de la matriz de asientos.
     */
    const std::string como_cadena() const;

    /**
     * Serializa la matriz de asientos.
     */
    const std::vector<uint8_t> serializar() const;

private:
    uint32_t n_asientos;
    uint32_t filas, columnas;
    std::vector<bool> asientos;
};

#endif // _MATRIZ_ASIENTOS_H_
