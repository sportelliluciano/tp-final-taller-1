#ifndef _TOSTADA_H_
#define _TOSTADA_H_

#include <string>

#include "cliente/video/ventana.h"

namespace cliente {

class Tostada {
public:
    /**
     * \brief Crea una nueva tostada.
     */
    Tostada(const std::string& mensaje, int duracion_);

    /**
     * \brief Renderiza la tostada.
     */
    void renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Devuelve true si termino el tiempo de la tostada.
     */
    bool esta_terminada() const;

    /**
     * \brief Devuelve el alto de la tostada.
     */
    int obtener_alto() const;

private:
    std::string texto;
    int duracion;
    bool terminada;
    int fin_ms;
};

} // namespace cliente

#endif // _TOSTADA_H_
