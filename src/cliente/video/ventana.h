#ifndef _VENTANA_H_
#define _VENTANA_H_

#include <SDL2/SDL.h>

#include <functional>
#include <unordered_map>

#include "cliente/video/administrador_texturas.h"

namespace cliente {

typedef enum {
    EVENTO_VENTANA_CERRAR
} evento_ventana_t;

/**
 * \brief Ventana. Administra escenas.
 */ 
class Ventana {
public:
    /**
     * \brief Crea una ventana con el tamaño por defecto.
     */
    Ventana();

    /**
     * \brief Crea una ventana de tamaño w x h.
     */
    Ventana(int w, int h);

    /**
     * \brief Actualiza la imagen mostrada en la ventana.
     * 
     * Todas las renderizaciones se realizan sobre un buffer oculto. Al 
     * finalizar el renderizado del cuadro actual se debe llamar a esta función
     * para mostrar el contenido del buffer oculto.
     */
    void actualizar();

    /**
     * \brief Devuelve el administrador de texturas de la ventana actual.
     */
    AdministradorTexturas& obtener_administrador_texturas();

    /**
     * \brief Devuelve el ancho en pixeles de la ventana.
     */
    int ancho() const;

    /**
     * \brief Devuelve el alto en pixeles de la ventana.
     */
    int alto() const;

    /**
     * \brief Devuelve la cantidad de veces que se renderizó la ventana durante
     *        el último segundo.
     */
    int fps() const;

    /**
     * \brief Registra un callback para un evento.
     * 
     * Sólo se permite un callback por evento.
     */
    void registrar_evento(evento_ventana_t evento, 
        std::function<void(void)> callback);

    /**
     * \brief Procesa los eventos que recibe la ventana.
     * 
     * Este método ejecuta los callbacks registrados.
     */
    void procesar_eventos();

    /**
     * \brief Destructor.
     */
    ~Ventana();

private:   
    /**
     * \brief Manejador de ventanas SDL.
     */
    SDL_Window *ventana;

    /**
     * \brief Renderer SDL
     */
    SDL_Renderer *renderer;

    /**
     * \brief Administradorde texturas de la ventana.
     */
    AdministradorTexturas *admin_texturas;

    /**
     * \brief Callbacks
     */
    std::unordered_map<evento_ventana_t, std::function<void(void)>> callbacks;

    /**
     * \brief Ticks en el último cuadro
     * 
     * Esta variable se utiliza para evitar renderizar más cuadros por segundo
     * que un determinado límite en el caso de no tener sincronizmo vertical.
     */
    Uint32 ticks_ultimo_cuadro;

    /**
     * \brief Sincronizmo vertical
     * 
     * Esta variable es true si el dispositivo soporta y está funcionando
     * con sincronizmo vertical.
     */
    bool vsync;

    /**
     * \brief Dimensiones de la ventana.
     */
    int ancho_px, alto_px;

    /**
     * \brief Cantidad de veces que se renderizó la ventana en el último 
     *        segundo.
     */
    unsigned int veces_renderizado, fps_, ticks_ultimo_segundo;

    /**
     * \brief Ejecuta todos los callbacks correspondientes al evento indicado.
     */
    void ejecutar_callbacks(evento_ventana_t evento);
};

} // namespace cliente

#endif // _VENTANA_H_
