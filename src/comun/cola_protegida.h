#ifndef _COLA_PROTEGIDA_H_
#define _COLA_PROTEGIDA_H_

#include <chrono>
#include <list>
#include <mutex>

#include "comun/lock.h"

namespace servidor {

/**
 * \brief Cola sincronizada de tipo push-pull.
 * 
 * Esta cola está protegida del acceso concurrente y funciona como una cola
 * push-pull.
 * Esto significa que intentar obtener un elemento de una cola vacía bloqueará
 * hasta que se agregue un nuevo elemento.
 * 
 * El tipo T debe ser copiable.
 */
template <class T>
class ColaProtegida {
public:
    /**
     * \brief Crea una nueva cola vacía.
     */
    ColaProtegida() {
        m_hay_datos.lock();
    }

    /**
     * \brief Agrega un nuevo elemento a la cola.
     * 
     * Este método es thread-safe.
     */
    void push(const T& data) {
        Lock l(m_cola);
        bool cola_estaba_vacia = cola.empty();
        cola.push_back(data);
        if (!desbloqueada && cola_estaba_vacia)
            m_hay_datos.unlock();
    }

    /**
     * \brief Obtiene un elemento de la cola.
     * 
     * Por defecto, si la cola está vacía esta función bloqueará hasta que se 
     * inserte un elemento. Si se setea bloquear en false la función lanzará
     * una excepción en caso de que esté vacía.
     *
     * Este método es thread-safe.
     */
    T pull(bool bloquear = true) {
        if (!desbloqueada && bloquear)
            m_hay_datos.lock();

        Lock l(m_cola);
        if (cola.empty())
            throw std::runtime_error("La cola está vacía");
        T data = cola.front();
        cola.pop_front();
        
        if (!desbloqueada && bloquear && !cola.empty())
            m_hay_datos.unlock();
        
        return data;
    }

    /**
     * \brief Intenta obtener un elemento de la cola
     * 
     * Si la cola tiene elementos, devuelve el primero. Si la cola no tiene 
     * elementos la función bloquea hasta que se inserte un elemento o hasta
     * que se llegue al punto en el tiempo especificado.
     * 
     * De llegar al momento del tiempo sin elementos devolverá un elemento
     * nuevo vacío.
     *
     * La función devuelve true si se puedo obtener un dato, o false en caso
     * contrario. El dato se devuelve por la interfaz.
     * Este método es thread-safe.
     */
    bool pull(std::chrono::steady_clock::time_point timeout, T& dato) {
        if (desbloqueada && cola.empty())
            return false;
        
        if (!desbloqueada && !m_hay_datos.try_lock_until(timeout))
            return false;

        Lock l(m_cola);
        dato = cola.front();
        cola.pop_front();
        
        if (!desbloqueada && !cola.empty())
            m_hay_datos.unlock();
        
        return true;
    }

    /**
     * \brief Devuelve true si la cola está vacía.
     * 
     * Este método es thread-safe.
     */
    bool esta_vacia() const {
        return cola.empty();
    }

    /**
     * \brief Desbloquea la cola provocando que los métodos pull lancen
     *        una excepción o devuelvan false.
     *
     * Desbloquear una cola ya desbloqueada no tiene ningún efecto.
     */
    void desbloquear() {
        Lock l(m_cola);
        if (desbloqueada)
            return;
        desbloqueada = true;
        m_hay_datos.unlock();
    }

private:
    std::mutex m_cola;
    std::timed_mutex m_hay_datos;
    std::list<T> cola;

    bool desbloqueada = false;
};

} // namespace servidor

#endif // _COLA_PROTEGIDA_H_
