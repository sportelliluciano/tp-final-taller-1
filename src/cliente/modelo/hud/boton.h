#ifndef _BOTON_H_
#define _BOTON_H_

#include <functional>

#include "cliente/modelo/sprite.h"
#include "cliente/video/rectangulo.h"

namespace cliente {

/**
 * \brief Boton del HUD.
 */
class Boton {
public:
    Boton();
    Boton(int sprite_id);

    void set_sprite(int sprite_id);

    void set_imagen(const char *img);

    void set_tamanio(int ancho, int alto);

    int obtener_alto() const;
    int obtener_ancho() const;

    void autoresize(bool activar);

    void registrar_click(std::function<void(void)> callback);

    /**
     * \brief Renderiza el botón en la posición indicada.
     */
    void renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Ejecuta el callback de click en el botón.
     */
    void click();

private:
    const char *imagen;
    int sprite;
    bool autoresize_activo = false;
    
    std::function<void(void)> cb_click = nullptr;

    Rectangulo boton;
};

} // namespace cliente

#endif // _BOTON_H_
