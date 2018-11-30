#include "cliente/sonido/sonido.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cliente/config.h"
#include "cliente/video/error_sdl.h"
#include "comun/log.h"

namespace cliente {

Sonido& Sonido::obtener_instancia() {
    static Sonido sonido;
    return sonido;
}

Sonido::Sonido() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        throw ErrorSDL("SDL_Init(SDL_INIT_AUDIO)");
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        throw ErrorSDL("Mix_OpenAudio", Mix_GetError());
    }

    volumen_sonidos = 1.0f;
}

void Sonido::reproducir_sonido(sonido_t sonido, int volumen) {
    if ((!sonido_habilitado) || (volumen == 0))
        return;
    
    if (sonidos.find(sonido) == sonidos.end()) {
        Mix_Chunk* nuevo_sonido = Mix_LoadWAV(RUTA_SONIDOS "/bleep.wav");
        if (!nuevo_sonido)
            throw ErrorSDL("Mix_LoadWAV", Mix_GetError());
        sonidos.emplace(sonido, nuevo_sonido);
    }

    Mix_Chunk *chunk = sonidos.at(sonido);

    float volumen_real = (volumen / 100.0f) * volumen_sonidos;

    Mix_VolumeChunk(chunk, (int)(MIX_MAX_VOLUME * volumen_real));

    if (Mix_PlayChannel(-1, chunk, 0) == -1)
        throw ErrorSDL("Mix_PlayChannel", Mix_GetError());
}

void Sonido::iniciar_musica_fondo() {
    if (musica_fondo == NULL) {
        musica_fondo = Mix_LoadMUS(RUTA_SONIDOS "/this_sick_beat.mp3");
        if (!musica_fondo)
            throw ErrorSDL("Mix_LoadMUS", Mix_GetError());
    }

    if (Mix_PlayingMusic() != 0)
        detener_musica_fondo();
    
    if (Mix_PausedMusic() != 0)
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(musica_fondo, -1);
}

void Sonido::detener_musica_fondo() {
    Mix_PauseMusic();
}

void Sonido::set_volumen_musica_fondo(int volumen) {
    if ((volumen < 0) || (volumen > MAX_VOLUMEN))
        throw std::runtime_error("set_volumen_musica_fondo: volumen inválido");
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (volumen / 100.0f)));
}

void Sonido::set_volumen_sonidos(int volumen) {
    if ((volumen < 0) || (volumen > MAX_VOLUMEN))
        throw std::runtime_error("set_volumen_sonidos: volumen inválido");
    volumen_sonidos = volumen / 100.0f;
    sonido_habilitado = (volumen != 0);
}

void Sonido::apagar() {
    log_depuracion("Limpiando Sonido", 0);
    Mix_HaltMusic();
    Mix_FreeMusic(musica_fondo);

    for (auto it=sonidos.begin(); it != sonidos.end(); ++it) {
        Mix_FreeChunk(it->second);
    }
    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

Sonido::~Sonido() {   
}

} // namespace cliente
