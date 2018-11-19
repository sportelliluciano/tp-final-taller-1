#include "cliente/video/ventana.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "cliente/video/administrador_texturas.h"
#include "cliente/video/camara.h"
#include "cliente/video/error_sdl.h"
#include "cliente/video/i_notificable.h"
#include "cliente/video/log.h"

#define ANCHO_VENTANA_DEFECTO 800
#define ALTO_VENTANA_DEFECTO  600


// Cuantos ticks ocurren por cada milisegundo de tiempo transcurrido
#define MS_POR_TICK 1

// 30ms ~ 30 FPS
#define TICKS_POR_CUADRO (30 / MS_POR_TICK)
#define TICKS_POR_SEGUNDO (1000 / MS_POR_TICK)

namespace cliente {

Ventana::Ventana() : Ventana(ANCHO_VENTANA_DEFECTO, ALTO_VENTANA_DEFECTO) { }

Ventana::Ventana(int w, int h, bool pantalla_completa, bool vsync_) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        throw ErrorSDL("SDL_Init");
    
    if (TTF_Init() != 0)
        throw ErrorSDL("TTF_Init", TTF_GetError());
    
    if (pantalla_completa) {
        ventana = SDL_CreateWindow("Dune Remake", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            0, 0, 
            SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        ventana = SDL_CreateWindow("Dune remake", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            w, h, 
            SDL_WINDOW_SHOWN);
    }
    
    if (!ventana)
        throw ErrorSDL("SDL_CreateWindow");

    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;
    
    if (vsync_)
        renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

    renderer = SDL_CreateRenderer(ventana, 0, renderer_flags);

    if (!renderer) {
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        throw ErrorSDL("SDL_CreateRenderer");
    }

    if (SDL_RenderClear(renderer) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        throw ErrorSDL("SDL_RenderClear");
    }
    
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        throw ErrorSDL("SDL_GetRenderInfo");
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    vsync = (info.flags & SDL_RENDERER_PRESENTVSYNC) ? true : false;
    admin_texturas = new AdministradorTexturas(renderer);
    
    SDL_GetWindowSize(ventana, &ancho_px, &alto_px);
    ancho_vp = ancho_px;
    alto_vp = alto_px;
    
    ticks_ultimo_cuadro = 0;
    ticks_ultimo_segundo = veces_renderizado = fps_ = 0;
    
    log_depuracion("VSync: %s", vsync ? "SI":"NO");
    log_depuracion("Tamaño de ventana: %dx%d", ancho_px, alto_px);
}

int Ventana::ancho() const {
    return ancho_vp;
}

int Ventana::alto() const {
    return alto_vp;
}

int Ventana::fps() const {
    return fps_;
}

uint64_t Ventana::obtener_ms() const {
    return ticks_ultimo_cuadro;
}

static tecla_t map_sdl_key(SDL_Keycode k) {
    switch(k) {
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            return TECLA_SHIFT;
        
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            return TECLA_CTRL;

        case SDLK_ESCAPE:
            return TECLA_ESCAPE;
        
        case SDLK_LEFT:
            return TECLA_IZQUIERDA;
        case SDLK_RIGHT:
            return TECLA_DERECHA;
        case SDLK_UP:
            return TECLA_ARRIBA;
        case SDLK_DOWN:
            return TECLA_ABAJO;
        
        case SDLK_F2:
            return TECLA_F2;
        
        default:
            return TECLA_NO_MAPEADA;
    }

    return TECLA_NO_MAPEADA;
}

void Ventana::registrar_notificable(INotificable& notificable) {
    receptor_eventos = &notificable;
}

// TODO: Refactorizar esto
void Ventana::procesar_eventos() {
    if (!receptor_eventos)
        return;
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        switch(evento.type) {
            case SDL_QUIT:
                receptor_eventos->cerrar_ventana();
                break;
            case SDL_WINDOWEVENT: 
                if (evento.window.event == SDL_WINDOWEVENT_ENTER) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    receptor_eventos->mouse_entra(Posicion(x, y));
                } else if (evento.window.event == SDL_WINDOWEVENT_LEAVE) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    receptor_eventos->mouse_sale(Posicion(x, y));
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (evento.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&mouse_inicio_arrastre_x, 
                        &mouse_inicio_arrastre_y);
                    mouse_down = true;
                }
                break;
            case SDL_MOUSEMOTION: {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (!mouse_arrastre_iniciado && mouse_down && (abs(x - mouse_inicio_arrastre_x) > 4) && (abs(x - mouse_inicio_arrastre_y) > 4)) {
                    mouse_arrastre_iniciado = true;
                    receptor_eventos->mouse_inicio_arrastre(Posicion(mouse_inicio_arrastre_x, 
                        mouse_inicio_arrastre_y));
                }
                receptor_eventos->mouse_movimiento(Posicion(evento.motion.x, 
                        evento.motion.y));
                } break;
            case SDL_MOUSEBUTTONUP: {
                    int x = evento.button.x,
                        y = evento.button.y;
                    
                    if (evento.button.button == SDL_BUTTON_LEFT) {
                        if (mouse_arrastre_iniciado) {
                            receptor_eventos->mouse_fin_arrastre(Posicion(x, y));
                            mouse_arrastre_iniciado = false;
                        } else {
                            receptor_eventos->mouse_click_izquierdo(Posicion(x, y));
                        }
                        mouse_down = false;
                    } else if (evento.button.button == SDL_BUTTON_RIGHT) {
                        receptor_eventos->mouse_click_derecho(Posicion(x, y));
                    }
                }
                break;
            case SDL_MOUSEWHEEL: {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    receptor_eventos->mouse_scroll(Posicion(x, y), evento.wheel.y);
                }
                break;
            case SDL_KEYDOWN: {
                tecla_t tecla = map_sdl_key(evento.key.keysym.sym);
                if (tecla != TECLA_NO_MAPEADA)
                    receptor_eventos->teclado_presionado(tecla);
                }
                break;
            case SDL_KEYUP: {
                tecla_t tecla = map_sdl_key(evento.key.keysym.sym);
                if (tecla != TECLA_NO_MAPEADA)
                    receptor_eventos->teclado_suelto(tecla);
                }
                break;
        }
    }
}

void Ventana::actualizar() {
    if (SDL_GetTicks() > ticks_ultimo_segundo + TICKS_POR_SEGUNDO) {
        ticks_ultimo_segundo = SDL_GetTicks();
        fps_ = veces_renderizado;
        veces_renderizado = 0;
    }

    if (!vsync) {
        /**
         * Si no hay sincronizmo vertical, limitar los FPS via software.
         */
        while (SDL_GetTicks() < ticks_ultimo_cuadro + TICKS_POR_CUADRO) {
            /**
             * SDL_Delay evita tener un consumo de CPU excesivo mientras el tiempo
             * transcurre.
             */
            SDL_Delay(1);
        }
    }

    SDL_RenderPresent(renderer);
    ticks_ultimo_cuadro = SDL_GetTicks();
    veces_renderizado++;
    
    // https://wiki.libsdl.org/SDL_RenderPresent recomienda 
    //  limpiar el backbuffer utilizando SDL_RenderClear.
    if (SDL_RenderClear(renderer) != 0)
        throw ErrorSDL("SDL_RenderClear");
}

AdministradorTexturas& Ventana::obtener_administrador_texturas() {
    return *admin_texturas;
}

// TODO: Sacar esto
void Ventana::dibujar_rectangulo(int x0, int y0, int x1, int y1, int color) {
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    if (color == 0)
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    else if (color == 1)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else if (color == 2)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else if (color == 3)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_Rect rc;
    rc.x = x0; rc.y = y0;
    rc.w = x1 - x0; rc.h = y1 - y0;
    SDL_RenderDrawRect(renderer, &rc);
    rc.x += 1; rc.y += 1; rc.w -= 2; rc.h -= 2;
    SDL_RenderDrawRect(renderer, &rc);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Ventana::dibujar_grilla(int x_offset, int y_offset) {
    if (x_offset > 0)
        x_offset = (x_offset % 32);
    else
        x_offset = -((-x_offset) % 32);
    
    if (y_offset > 0)
        y_offset = (y_offset % 32);
    else
        y_offset = -((-y_offset) % 32);

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    /******** GRILLA DEPURACION *********/
    for (int i=0; i<=ancho() / 32; i++) {
        SDL_RenderDrawLine(renderer, 
            x_offset + i*32, y_offset, 
            x_offset + i*32, alto() + y_offset);
        for (int j=0; j<= alto() / 32; j++) {
            SDL_RenderDrawLine(renderer, 
                x_offset, j * 32 + y_offset, 
                ancho() + x_offset, j * 32 + y_offset);
        }
    }
    /************************************/
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Ventana::setear_viewport(const Rectangulo& seccion) {
    if (SDL_RenderSetViewport(renderer, &seccion.rect()) != 0)
        throw ErrorSDL("SDL_RenderSetViewport");
    
    ancho_vp = seccion.ancho();
    alto_vp = seccion.alto();
}

void Ventana::reestablecer_viewport() {
    if (SDL_RenderSetViewport(renderer, NULL) != 0)
        throw ErrorSDL("SDL_RenderSetViewport");
    
    ancho_vp = ancho_px;
    alto_vp = alto_px;
}

void Ventana::ocultar_mouse() {
    SDL_ShowCursor(SDL_DISABLE);
}

void Ventana::mostrar_mouse() {
    SDL_ShowCursor(SDL_ENABLE);
}

Ventana::~Ventana() {
    delete admin_texturas;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

} // namespace cliente
