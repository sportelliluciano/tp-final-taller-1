#ifndef _VENTANA_H_
#define _VENTANA_H_

#include <SDL2/SDL.h>

#include <vector>
#include <utility>

#include "cliente/video/administrador_texturas.h"
#include "cliente/video/camara.h"
#include "cliente/video/i_notificable.h"
#include "cliente/video/color.h"
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
     * \brief Crea una nueva ventana.
     * 
     * La nueva ventana creada será de w x h.
     * 
     * Si pantalla_completa es true, se ignorará el tamaño pasado y se mostrará
     * la ventana a pantalla completa, en la resolución nativa.
     * 
     * Si vsync es true, se intentará utilizar sincronizmo vertical. Si es 
     * false se desactivará el sincronizmo vertical (aunque esté disponible).
     */
    Ventana(int w, int h, bool pantalla_completa = false, bool vsync_ = true);

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
    uint64_t obtener_ms() const;

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
     * 
     * Si no hay ningún callback registrado, se procesará únicamente el evento
     * "salir" y devolverá false en caso de que se haya detectado dicho evento.
     * En cualquier otro caso devolverá true.
     */
    bool procesar_eventos();

    /**
     * \brief Dibuja un rectángulo sobre la ventana
     */
    void dibujar_rectangulo(int x0, int y0, int x1, int y1, const Color& color);

    /**
     * \brief Dibuja una poligonal a partir de sus vértices.
     */
    void dibujar_poligonal(const std::vector<std::pair<int, int>> linea,
        int trasladar_x, int trasladar_y);

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
     * \brief Oculta el cursor del mouse sobre la ventana.
     */
    void ocultar_mouse();

    /**
     * \brief Muestra el cursor del mouse sobre la ventana.
     */
    void mostrar_mouse();

    /**
     * \brief Cambia el renderizado al plano frontal o al plano trasero.
     *
     * Si frontal es true, los renderizados siguientes se realizarán sobre
     * el plano frontal; caso contrario se realizarán sobre el plano trasero.
     */
    void cambiar_plano(bool frontal);

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
     * \brief Viewport actual.
     */
    Rectangulo viewport;

    /**
     * \brief Cantidad de veces que se renderizó la ventana en el último 
     *        segundo.
     */
    unsigned int veces_renderizado, fps_, ticks_ultimo_segundo;

    bool mouse_arrastre_iniciado = false, mouse_down = false;
    int mouse_inicio_arrastre_x, mouse_inicio_arrastre_y;
    
    bool en_plano_frontal = false;
    SDL_Texture* plano_frontal = nullptr;
};

} // namespace cliente

#endif // _VENTANA_H_

