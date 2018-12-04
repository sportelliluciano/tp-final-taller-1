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
    CELDA_ROCA,       // Se puede construir y caminar sobre este terreno
    CELDA_ARENA,      // Sólo se puede caminar sobre este terreno.
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
    bool es_caminable() const;
    bool es_construible();
    bool hay_tropa() const;
    void agregar_tropa();
    void eliminar_tropa();

private:
    int posicion_x, posicion_y;
    tipo_celda_t tipo_celda = CELDA_ARENA;
    
    /**
     * \brief Tropas paradas en la celda actual.
     */
    bool tropa = false;
    bool edificio = false;
    /**
     * \brief Si hay un edificio en esta celda entonces este puntero 
     *        apuntará al mismo.
     * 
     * Sólo puede haber un edificio por celda.
     */
    //const Edificio *edificio = nullptr;

    /**
     * \brief Identificador único de celda.
     */
};

} // namespace modelo

#endif // _CELDA_H_
