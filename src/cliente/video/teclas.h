#ifndef _TECLAS_H_
#define _TECLAS_H_

/**
 * Este archivo define el tipo enumerativo tecla_t
 */

namespace cliente {

typedef enum {
    TECLA_NO_MAPEADA,
    TECLA_ESCAPE,
    TECLA_SHIFT,
    TECLA_ARRIBA,
    TECLA_ABAJO,
    TECLA_DERECHA,
    TECLA_IZQUIERDA
} tecla_t; 

} // namespace cliente

#endif // _TECLAS_H_
