#include "cliente/video/ventana.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
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
        SDL_Quit();
        throw ErrorSDL("SDL_CreateRenderer");
    }

    if (SDL_RenderClear(renderer) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        throw ErrorSDL("SDL_RenderClear");
    }
    
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
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

void Ventana::registrar_evento(evento_ventana_t evento, 
    std::function<void(void)> callback) {
    callbacks[evento] = callback;
}

void Ventana::ejecutar_callbacks(evento_ventana_t evento) {
    if (callbacks.count(evento) != 0)
        callbacks.find(evento)->second();
}

void Ventana::procesar_eventos() {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT)
            ejecutar_callbacks(EVENTO_VENTANA_CERRAR);
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
        
        ticks_ultimo_cuadro = SDL_GetTicks();
    }

    /******** GRILLA DEPURACION *********/
    for (int i=0;i<ancho() / 32; i++) {
        SDL_RenderDrawLine(renderer, i*32, 0, i*32, alto());
        for (int j=0;j<=alto() / 32; j++) {
            SDL_RenderDrawLine(renderer, 0, j * 32, ancho(), j * 32);
        }
    }
    /************************************/

    SDL_RenderPresent(renderer);
    veces_renderizado++;
    
    // https://wiki.libsdl.org/SDL_RenderPresent recomienda 
    //  limpiar el backbuffer utilizando SDL_RenderClear.
    if (SDL_RenderClear(renderer) != 0)
        throw ErrorSDL("SDL_RenderClear");
}

AdministradorTexturas& Ventana::obtener_administrador_texturas() {
    return *admin_texturas;
}

Ventana::~Ventana() {
    delete admin_texturas;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    SDL_Quit();
}

} // namespace cliente
