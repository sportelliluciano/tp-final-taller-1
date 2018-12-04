#ifndef _LOCK_H_
#define _LOCK_H_

#include <mutex>

/**
 * Encapsulamiento RAII para un mutex. 
 * 
 * Permite bloquear un mutex dentro de un scope y desbloquearlo automáticamente
 * al salir de dicho scope.
 */
class Lock {
public:
    /**
     * Constructor. 
     * 
     * Bloquea el mutex pasado por parámetro.
     */
    explicit Lock(std::mutex& m);

    /**
     * Destructor.
     * 
     * Desbloquea el mutex bloqueado en el constructor.
     */
    ~Lock();

private:
    std::mutex& mutex;
    /* Constructor por copia */
    Lock(const Lock &otro) = delete;
    /* Constructor por movimiento */
    Lock(Lock&& otro) = delete;
    /* Asignación por copia */
    Lock& operator=(const Lock &otro) = delete;
    /* Asignación por movimiento */
    Lock& operator=(Lock&& otro) = delete;
};

#endif // _LOCK_H_
