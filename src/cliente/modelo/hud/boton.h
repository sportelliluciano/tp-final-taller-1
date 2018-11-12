#ifndef _BOTON_H_
#define _BOTON_H_

#include <functional>

#include "cliente/modelo/sprite.h"
#include "cliente/video/rectangulo.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Boton del HUD.
 */
class Boton : public Widget {
public:
    Boton();
    Boton(int sprite_id);

    void set_sprite(int sprite_id);

    void set_imagen(const char *img);

    void set_tamanio(int ancho, int alto);

    void set_padding(int x, int y);

    int obtener_alto() const override;
    
    int obtener_ancho() const override;

    void autoresize(bool activar);
    void set_autopadding(bool activar);

    void registrar_click(std::function<void(void)> callback);

    /**
     * \brief Renderiza el botón en la posición indicada.
     */
    void renderizar(Ventana& ventana, int x, int y);

    bool mouse_click_izquierdo(int x, int y) override;

private:
    const char *imagen;
    int sprite;
    bool autoresize_activo = false;
    bool autopadding = false;
    
    int padding_x = 0, padding_y = 0;

    std::function<void(void)> cb_click = nullptr;

    Rectangulo boton;
};

} // namespace cliente

#endif // _BOTON_H_
