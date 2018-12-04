#ifndef _BARRA_VIDA_H_
#define _BARRA_VIDA_H_

#include "cliente/video/ventana.h"

namespace cliente {

class BarraVida {
public:
    /**
     * \brief Crea una nueva barra de vida.
     */
    BarraVida();

    /**
     * \brief Setea el ancho de la barra de vida.
     * 
     * Si el ancho es menor al mínimo se ignorará y se utilizará el mínimo.
     */
    void set_ancho(int nuevo_ancho);

    /**
     * \brief Renderiza la barra de vida.
     */
    void renderizar(Ventana& ventana, int x, int y, int vida, int maximo);

private:
    int ancho;

    /**
     * \brief Obtiene la textura de la barra de vida.
     * 
     * Si la textura está en caché se reutilizará. En caso contrario se
     * creará.
     */
    Textura& obtener_textura(Ventana& ventana, int vida, int max);

    /**
     * \brief Crea la textura de la barra de vída.
     * 
     * Almacena la textura creada en la caché de texturas de la ventana.
     */
    Textura& crear_barra_vida(AdministradorTexturas& admin_texturas, 
        const std::string& id_textura, int vida, int max);
};

} // namespace cliente

#endif // _BARRA_VIDA_H_
