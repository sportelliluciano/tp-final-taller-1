#ifndef _CELDA_H_
#define _CELDA_H_

#include <vector>

#define ANCHO_CELDA 8
#define ALTO_CELDA 8

namespace modelo {

/**
 * \brief Tipos de celda.
 */
typedef enum {
    CELDA_ARENA,      // Sólo se puede caminar sobre este terreno.
    CELDA_ROCA,       // Se puede construir y caminar sobre este terreno
    CELDA_PRECIPICIO, // Barrera natural.
    CELDA_DUNA,       // Se puede caminar por este terreno a menos velocidad
    CELDA_ESPECIA     // Especia recolectable.
} tipo_celda_t;

/**
 * \brief Celda del terreno. 
 * 
 * Representa una unidad mínima del terreno.
 */
class Celda { 
public:
    /**
     * \brief Constructor por defecto.
     */
    Celda() = default;
    Celda(tipo_celda_t tipo,int x_, int y_);

    int tipo() const;
    int x() const;
    int y() const;

    bool tiene_edificio() const;
    void agregar_edificio();
    void eliminar_edificio();
    /**
     * \brief Devuelve true en caso de que la celda sea caminable.
     * Entiendase por caminable una celda de tipo ARENA, ESPECIA.
     * ROCA o DUNA que este libre (sin edificios ni otras tropas).
     */
    bool es_caminable() const;
    /**
     * \brief Devuelve true en caso de que la celda sea construible.
     * Entiendase por caminable una celda de tipo ROCA que este libre
     * (sin edificios ni otras tropas).
     */
    bool es_construible();
    bool hay_tropa() const;
    void agregar_tropa();
    void eliminar_tropa();

private:
    int posicion_x, posicion_y;
    tipo_celda_t tipo_celda = CELDA_ARENA;
    /**
     * \brief Unidad paradas en la celda actual.
     */
    bool tropa = false;
    /**
     * \brief Edicifio construido en la celda actual.
     */
    bool edificio = false;
};

} // namespace modelo

#endif // _CELDA_H_
