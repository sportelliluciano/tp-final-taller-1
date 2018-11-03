#ifndef _VENTANA_H_
#define _VENTANA_H_

#include <SDL2/SDL.h>

#include <functional>
#include <unordered_map>

#include "cliente/video/administrador_texturas.h"

namespace cliente {

typedef enum {
    BOTON_DERECHO,
    BOTON_CENTRAL,
    BOTON_IZQUIERDO
} boton_mouse_t;

typedef enum {
    TECLA_NO_MAPEADA,
    TECLA_ESCAPE,
    TECLA_SHIFT
} tecla_t;

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
     * \brief Registra un callback para el cierre de la ventana
     * 
     * Sólo se permite un callback por evento.
     */
    void registrar_ventana_cerrar(std::function<void(void)> callback);

    /**
     * \brief Registra un callback para los eventos de click del mouse.
     * 
     * El evento mouse_down se ejecuta cuando se presiona algún botón del mouse;
     * el evento mouse_up se ejecuta cuando se suelta el botón.
     * 
     * Se puede pasar nullptr si no se desea controlar algún evento o para
     * des-registrar los callbacks.
     * 
     * Sólo se permite un callback por evento.
     */
    void registrar_mouse_click(
        std::function<void(boton_mouse_t, int, int)> mouse_down,
        std::function<void(boton_mouse_t, int, int)> mouse_up);
    
    void registrar_mouse_motion(std::function<void(int, int)> mouse_motion);
    
    /**
     * \brief Registra un callback para los eventos de rueda del mouse.
     * 
     * Este evento ocurre cuando el usuario hace scroll con la rueda del mouse.
     * 
     * El callback recibe un parámetro: 
     *  void rueda_callback(int y)
     * - y: Cuántos píxeles se scrollearon verticalmente. El signo indica la
     *      dirección.
     * 
     * Se puede pasar nullptr si no se desea controlar algún evento o para
     * des-registrar los callbacks.
     * 
     * Sólo se permite un callback por evento.
     */
    void registrar_rueda_mouse(std::function<void(int)> rueda_callback);

    /**
     * \brief Registra un callback para los eventos del teclado.
     * 
     * El evento teclado_down se ejecuta cuando se empieza a presionar una tecla;
     * el evento teclado_up se ejecuta cuando se suelta la tecla.
     * 
     * Se puede pasar nullptr si no se desea controlar algún evento o para
     * des-registrar los callbacks.
     * 
     * Sólo se notificarán las teclas que están registradas en el tipo tecla_t.
     * 
     * Sólo se permite un callback por evento.
     */
    void registrar_teclado(
        std::function<void(tecla_t)> teclado_down,
        std::function<void(tecla_t)> teclado_up);

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

    std::function<void(void)> cb_ventana_cerrar = nullptr;
    std::function<void(boton_mouse_t, int, int)> cb_mouse_click_up = nullptr,
        cb_mouse_click_down = nullptr;
    std::function<void(tecla_t)> cb_teclado_up = nullptr,
        cb_teclado_down = nullptr;
    std::function<void(int)> cb_rueda_mouse = nullptr;
    std::function<void(int, int)> cb_mouse_motion = nullptr;
};

} // namespace cliente

#endif // _VENTANA_H_

