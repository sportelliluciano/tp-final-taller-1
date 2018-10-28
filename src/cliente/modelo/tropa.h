#ifndef _TROPA_H_
#define _TROPA_H_

#include "cliente/video/ventana.h"

namespace cliente {

class Tropa {
public:
    /**
     * \brief Renderiza la tropa.
     */
    virtual void renderizar(Ventana& ventana) = 0;

    /**
     * \brief Destructor virtual.
     */
    virtual ~Tropa() { }
};

} // namespace cliente

#endif // _TROPA_H_
