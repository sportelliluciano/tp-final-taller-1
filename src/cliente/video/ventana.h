#ifndef _VENTANA_H_
#define _VENTANA_H_

#include <SDL2/SDL.h>

#include <functional>
#include <unordered_map>

#include "cliente/video/administrador_texturas.h"
#include "cliente/video/i_notificable.h"
#include "cliente/video/teclas.h"

namespace cliente {

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
     * \brief Devuelve el timestamp en milisegundos del último cuadro 
     *        presentado.
     * 
     * Devuelve la cantidad de milisegundos que pasaron hasta el último cuadro
     * presentado via Ventana::actualizar.
     */
    int obtener_ms() const;

    /**
     * \brief Registra un receptor de eventos de la ventana.
     * 
     * Sólo puede haber un receptor de eventos de la ventana.
     */
    void registrar_notificable(INotificable& notificable);

    /**
     * \brief Procesa los eventos que recibe la ventana.
     * 
     * Este método ejecuta los callbacks registrados.
     */
    void procesar_eventos();

    /**
     * \brief Dibuja un rectángulo de depuración sobre la ventana
     * TODO: Arreglar esto
     */
    void dibujar_rectangulo(int x0, int y0, int x1, int y1, int color = 0);

    /**
     * \brief Dibuja una grilla de 32x32 sobre la ventana.
     * TODO: Eliminar esto
     */
    void dibujar_grilla();

    /**
     * \brief Setea el viewport de la ventana al rectángulo indicado.
     * 
     * Una vez seteado el viewport, la ventana sólo renderizará dentro del 
     * rectángulo indicado, siendo la posición (0, 0) la posición indicada como
     * (x, y) del rectángulo. Adicionalmente setear el viewport provoca que
     * los métodos ancho y alto devuelvan el ancho y alto del nuevo viewport.
     */
    void setear_viewport(const Rectangulo& seccion);

    /**
     * \brief Reestablece el viewport a su valor por defecto.
     */
    void reestablecer_viewport();

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
     * \brief Objeto que recibe los eventos generados por la ventana.
     */
    INotificable* receptor_eventos = nullptr;

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
     * \brief Dimensiones del viewport.
     */
    int ancho_vp, alto_vp;

    /**
     * \brief Cantidad de veces que se renderizó la ventana en el último 
     *        segundo.
     */
    unsigned int veces_renderizado, fps_, ticks_ultimo_segundo;

    bool mouse_arrastre_iniciado = false, mouse_down = false;
    int mouse_inicio_arrastre_x, mouse_inicio_arrastre_y;
    
};

} // namespace cliente

#endif // _VENTANA_H_

