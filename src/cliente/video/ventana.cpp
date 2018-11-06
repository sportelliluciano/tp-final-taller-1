#include "cliente/video/ventana.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "cliente/video/administrador_texturas.h"
#include "cliente/video/error_sdl.h"
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

Ventana::Ventana(int w, int h) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        throw ErrorSDL("SDL_Init");
    
    if (TTF_Init() != 0)
        throw ErrorSDL("TTF_Init", TTF_GetError());
    
    ventana = SDL_CreateWindow("Dune remake", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    
    if (!ventana)
        throw ErrorSDL("SDL_CreateWindow");
    
    renderer = SDL_CreateRenderer(ventana, 0, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

    vsync = (info.flags & SDL_RENDERER_PRESENTVSYNC) ? true : false;
    admin_texturas = new AdministradorTexturas(renderer);
    ticks_ultimo_cuadro = 0;
    log_depuracion("VSync: %s", vsync ? "SI":"NO");
    ancho_px = w;
    alto_px = h;
    ticks_ultimo_segundo = veces_renderizado = fps_ = 0;
}

int Ventana::ancho() const {
    return ancho_px;
}

int Ventana::alto() const {
    return alto_px;
}

int Ventana::fps() const {
    return fps_;
}

int Ventana::obtener_ms() const {
    return ticks_ultimo_cuadro;
}

void Ventana::registrar_ventana_cerrar(std::function<void(void)> callback) {
    cb_ventana_cerrar = callback;
}

void Ventana::registrar_mouse_click(
    std::function<void(boton_mouse_t, int, int)> mouse_down,
    std::function<void(boton_mouse_t, int, int)> mouse_up)
{
    cb_mouse_click_down = mouse_down;
    cb_mouse_click_up = mouse_up;
}

void Ventana::registrar_rueda_mouse(
    std::function<void(int)> rueda_callback)
{
    cb_rueda_mouse = rueda_callback;
}

void Ventana::registrar_mouse_motion(std::function<void(int, int)> callback) {
    cb_mouse_motion = callback;
}

void Ventana::registrar_teclado(
    std::function<void(tecla_t)> teclado_down,
    std::function<void(tecla_t)> teclado_up) 
{
    cb_teclado_down = teclado_down;
    cb_teclado_up = teclado_up;
}

static tecla_t map_sdl_key(SDL_Keycode k) {
    switch(k) {
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            return TECLA_SHIFT;
        
        case SDLK_ESCAPE:
            return TECLA_ESCAPE;
        
        default:
            return TECLA_NO_MAPEADA;
    }

    return TECLA_NO_MAPEADA;
}

void Ventana::procesar_eventos() {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        switch(evento.type) {
            case SDL_QUIT:
                if (cb_ventana_cerrar)
                    cb_ventana_cerrar();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (cb_mouse_click_down) {
                    boton_mouse_t boton = BOTON_IZQUIERDO;
                    if (evento.button.button == SDL_BUTTON_RIGHT)
                        boton = BOTON_DERECHO;
                    else if (evento.button.button == SDL_BUTTON_MIDDLE)
                        boton = BOTON_CENTRAL;
                    
                    cb_mouse_click_down(boton, evento.button.x, evento.button.y);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (cb_mouse_click_up) {
                    boton_mouse_t boton = BOTON_IZQUIERDO;
                    if (evento.button.button == SDL_BUTTON_RIGHT)
                        boton = BOTON_DERECHO;
                    else if (evento.button.button == SDL_BUTTON_MIDDLE)
                        boton = BOTON_CENTRAL;
                    
                    cb_mouse_click_up(boton, evento.button.x, evento.button.y);
                }
                break;
            case SDL_MOUSEWHEEL:
                if (cb_rueda_mouse) {
                    cb_rueda_mouse(evento.wheel.y);
                }
                break;
            case SDL_MOUSEMOTION:
                if (cb_mouse_motion) {
                    cb_mouse_motion(evento.motion.x, evento.motion.y);
                }
                break;
            case SDL_KEYDOWN: {
                tecla_t tecla = map_sdl_key(evento.key.keysym.sym);
                if (tecla != TECLA_NO_MAPEADA)
                    cb_teclado_down(tecla);
                }
                break;
            case SDL_KEYUP: {
                tecla_t tecla = map_sdl_key(evento.key.keysym.sym);
                if (tecla != TECLA_NO_MAPEADA)
                    cb_teclado_up(tecla);
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
void Ventana::dibujar_rectangulo(int x0, int y0, int x1, int y1) {
    if ((abs(x1 - x0) * abs(y1 - y0)) < 32)
        return;
    
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
    SDL_Rect rc;
    rc.x = x0; rc.y = y0;
    rc.w = x1 - x0; rc.h = y1 - y0;
    SDL_RenderDrawRect(renderer, &rc);
    rc.x += 1; rc.y += 1; rc.w -= 2; rc.h -= 2;
    SDL_RenderDrawRect(renderer, &rc);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Ventana::dibujar_grilla() {
    /******** GRILLA DEPURACION *********/
    for (int i=0;i<ancho() / 32; i++) {
        SDL_RenderDrawLine(renderer, i*32, 0, i*32, alto());
        for (int j=0;j<=alto() / 32; j++) {
            SDL_RenderDrawLine(renderer, 0, j * 32, ancho(), j * 32);
        }
    }
    /************************************/
}

Ventana::~Ventana() {
    delete admin_texturas;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

} // namespace cliente
