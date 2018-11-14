#include "servidor/cola.h"

#include <list>
#include <mutex>

#include "libs/json.hpp"

#include "conexion/lock.h"

namespace servidor {

Cola::Cola() { 
    // Inicialmente la cola está vacía.
    m_hay_datos.lock();
}

/**
 * Los métodos push y pull usan el mutex m_hay_datos para que pull se bloquee
 * hasta que un nuevo dato llegue a la cola.
 * 
 * El mutex m_cola protege a la estructura de datos del acceso concurrente.
 */
void Cola::push(const nlohmann::json& data) {
    Lock l(m_cola);
    bool cola_estaba_vacia = cola.empty();
    cola.push_back(data);
    if (cola_estaba_vacia)
        m_hay_datos.unlock();
}

nlohmann::json Cola::pull() {
    m_hay_datos.lock();

    Lock l(m_cola);
    nlohmann::json data = cola.back();
    cola.pop_back();
    
    if (!cola.empty())
        m_hay_datos.unlock();
    
    return data;
}

bool Cola::esta_vacia() const {
    return cola.empty();
}

} // namespace servidor