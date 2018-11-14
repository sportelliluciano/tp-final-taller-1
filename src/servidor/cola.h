#ifndef _COLA_H_
#define _COLA_H_

#include <list>
#include <mutex>

#include "libs/json.hpp"

namespace servidor {

/**
 * \brief Cola sincronizada de tipo push-pull.
 * 
 * Esta cola está protegida del acceso concurrente y funciona como una cola
 * push-pull.
 * Esto significa que intentar obtener un elemento de una cola vacía bloqueará
 * hasta que se agregue un nuevo elemento.
 */
class Cola {
public:
    /**
     * \brief Crea una nueva cola vacía.
     */
    Cola();

    /**
     * \brief Agrega un nuevo elemento a la cola.
     * 
     * Este método es thread-safe.
     */
    void push(const nlohmann::json& data);

    /**
     * \brief Obtiene un elemento de la cola.
     * 
     * Si la cola está vacía esta función bloqueará hasta que se inserte un
     * elemento.
     *
     * Este método es thread-safe.
     */
    nlohmann::json pull();

    /**
     * \brief Devuelve true si la cola está vacía.
     * 
     * Este método es thread-safe.
     */
    bool esta_vacia() const;

private:
    std::mutex m_cola, m_hay_datos;
    std::list<nlohmann::json> cola;
};

} // namespace servidor

#endif // _COLA_H_
