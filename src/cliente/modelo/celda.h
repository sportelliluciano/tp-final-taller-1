#ifndef _CELDA_H_
#define _CELDA_H_

#include <list>
#include <vector>

#include "cliente/modelo/edificio.h"
#include "cliente/modelo/tropa.h"
#include "cliente/video/ventana.h"

namespace cliente {

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
 * \brief Celda lógica del tablero. 
 * 
 * Representa una celda de 32x32 del tablero de juego.
 */
class Celda { 
public:
    Celda() = default;

    /**
     * \brief Constructor desde tipo. 
     * 
     * Crea una nueva celda del tipo indicado.
     */
    Celda(tipo_celda_t tipo);

    /**
     * \brief Renderiza la celda sobre la textura destino
     */
    void renderizar(Ventana& ventana, int x, int y, Textura& destino);

    /**
     * \brief Devuelve true si se puede construir sobre la celda.
     */
    bool es_construible() const;

    /**
     * \brief Devuelve true si hay al menos una tropa en la celda actual.
     */
    bool contiene_tropas() const;

    /**
     * \brief Devuelve true si hay un edificio construido sobre la celda actual.
     */
    bool contiene_edificio() const;

    /**
     * \brief Devuelve las tropas que estén paradas sobre la celda actual.
     */
    const std::list<Tropa*>& obtener_tropas() const;

    /**
     * \brief Setea el edificio construido sobre la celda.
     * 
     * Si la celda ya contenía un edificio o contenía tropas lanzará una 
     * excepción de tipo runtime_error.
     */
    void set_edificio(Edificio& nuevo_edificio);

    /**
     * \brief Obtiene el edificio que contiene la celda.
     * 
     * Si la celda no contiene ningún edificio lanza runtime_error.
     */
    Edificio& obtener_edificio();

    /**
     * \brief Elimina el edificio de la celda.
     * 
     * Si el edificio a eliminar no es el mismo que el eliminado o no habia
     * un edificio en la celda, lanzará una excepción de tipo runtime_error.
     */
    void eliminar_edificio(const Edificio& edificio_eliminado);

    /**
     * \brief Agrega una tropa a la celda actual.
     * 
     * Si la celda ya contenía un edificio lanzará una excepción de tipo 
     * runtime_error.
     */
    void agregar_tropa(Tropa& tropa);

    /**
     * \brief Elimina una tropa de la celda actual.
     * 
     * Si la celda no contenía tropas o la tropa no estaba lanza runtime_error.
     * 
     * Esta operación es O(k) con k la cantidad de tropas en la celda.
     */
    void eliminar_tropa(const Tropa& tropa);

private:
    /**
     * \brief Indica de que tipo es la celda.
     */
    tipo_celda_t tipo_celda = CELDA_ARENA;
    
    /**
     * \brief Tropas paradas en la celda actual.
     */
    std::list<Tropa*> tropas;

    /**
     * \brief Si hay un edificio en esta celda entonces este puntero 
     *        apuntará al mismo.
     * 
     * Sólo puede haber un edificio por celda.
     */
    Edificio *edificio = nullptr;

    /**
     * \brief Identificador único de celda.
     */
    int id_celda;
};

} // namespace cliente


#endif // _CELDA_H_
