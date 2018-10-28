#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <vector>

#include "cliente/modelo/celda.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Interfaz edificio.
 */
class Edificio {
public:
    /**
     * \brief Devuelve las celdas que ocupa el edificio
     */
    virtual std::vector<Celda> obtener_celdas_ocupadas() = 0;

    /**
     * \brief Renderiza el edificio.
     */
    virtual void renderizar(Ventana& ventana) = 0;
   
    /**
     * \brief Destructor virtual.
     */
    virtual ~Edificio() { }
};

} // namespace cliente

#endif // _EDIFICIO_H_
