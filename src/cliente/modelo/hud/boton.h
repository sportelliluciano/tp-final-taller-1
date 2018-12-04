#ifndef _BOTON_H_
#define _BOTON_H_

#include <functional>

#include "cliente/modelo/sprite.h"
#include "cliente/video/rectangulo.h"
#include "cliente/video/widgets/widget.h"

namespace cliente {

/**
 * \brief Botón genérico con imagen.
 */
class Boton : public Widget {
public:
    /**
     * \brief Crea un nuevo botón.
     */
    Boton();

    /**
     * \brief Setea la imagen del botón.
     */
    void set_imagen(const char *img);

    /**
     * \brief Setea el tamaño del botón. 
     * 
     * La imagen del mismo se escalará de forma acorde.
     */
    void set_tamanio(int ancho, int alto);

    /**
     * \brief Agrega un padding al botón.
     * 
     * El padding es el espacio que hay entre el borde del botón y la imagen.
     */
    void set_padding(int x, int y);

    /**
     * \brief Centra la imagen vertical y horizontalmente en el botón.
     */
    void centrar_imagen(bool activar);

    /**
     * \brief Callback para cuando el usuario hace clic con el botón 
     *        izquierdo del mouse en el botón.
     */
    void registrar_click(std::function<void(void)> callback);

    /** Interfaz Widget **/
    int obtener_alto() const override;
    int obtener_ancho() const override;
    void renderizar(Ventana& ventana, const Posicion& punto) override;
    bool mouse_click_izquierdo(const Posicion& punto) override;

private:
    const char *imagen;
    bool autopadding = false;
    
    int padding_x = 0, padding_y = 0;

    std::function<void(void)> cb_click = nullptr;

    Rectangulo boton;
};

} // namespace cliente

#endif // _BOTON_H_
