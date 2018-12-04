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
    int obtener_alto(Ventana& ventana) const;

private:
    std::string texto, id_textura;
    int duracion;
    bool terminada;
    uint64_t fin_ms;
    bool iniciada;

    /**
     * \brief Renderiza la tostada sobre una nueva textura y la devuelve.
     * 
     * La textura creada se almacenará en la caché de la ventana.
     */
    Textura& crear_textura_tostada(Ventana& ventana) const;

    /**
     * \brief Obtiene la textura de la tostada.
     * 
     * Si la textura no había sido creada se creará.
     */
    Textura& obtener_textura(Ventana& ventana) const;
    
};

} // namespace cliente

#endif // _TOSTADA_H_
