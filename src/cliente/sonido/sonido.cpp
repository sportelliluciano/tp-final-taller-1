#include "cliente/sonido/sonido.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cliente/video/error_sdl.h"

namespace cliente {

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
    if (sonidos.find(sonido) == sonidos.end()) {
        Mix_Chunk* nuevo_sonido = Mix_LoadWAV("./assets/sonidos/bleep.wav");
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

void Sonido::iniciar_musica_fondo(musica_t musica) {
    if (musicas.find(musica) == musicas.end()) {
        Mix_Music* nueva_musica = Mix_LoadMUS("./assets/sonidos/this_sick_beat.mp3");
        if (!nueva_musica)
            throw ErrorSDL("Mix_LoadMUS", Mix_GetError());
        musicas.emplace(musica, nueva_musica);
    }

    if (Mix_PlayingMusic() != 0)
        detener_musica_fondo();
    
    Mix_PlayMusic(musicas.at(musica), -1);
}

void Sonido::detener_musica_fondo() {
    Mix_HaltMusic();
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
}

Sonido::~Sonido() {
    Mix_HaltMusic();
    
    for (auto it=sonidos.begin(); it != sonidos.end(); ++it) {
        Mix_FreeChunk(it->second);
    }

    for (auto it=musicas.begin(); it != musicas.end(); ++it) {
        Mix_FreeMusic(it->second);
    }

    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

} // namespace cliente
