#include "cliente/sonido/sonido.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cliente/config.h"
#include "cliente/video/error_sdl.h"
#include "comun/log.h"

#define MS_POR_SEG 1000
#define MUESTRAS_POR_CHUNK 4096
#define FRECUENCIA_AUDIO 22050
#define N_CANALES 2
#define BYTES_POR_MUESTRA 2

#define FORMATO_AUDIO ((FRECUENCIA_AUDIO * N_CANALES * BYTES_POR_MUESTRA))

namespace cliente {

Sonido& Sonido::obtener_instancia() {
    static Sonido sonido;
    return sonido;
}

void Sonido::reproducir_sonido(sonido_t sonido) {
    obtener_instancia().reproducir_sonido_(sonido);
}

Sonido::Sonido() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        log_error("Sonido desactivado: %s", SDL_GetError());
        return;
    }

    if (Mix_OpenAudio(FRECUENCIA_AUDIO, MIX_DEFAULT_FORMAT, N_CANALES, 
        MUESTRAS_POR_CHUNK) != 0) 
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        log_error("Sonido desactivado: %s", Mix_GetError());
        return;
    }

    subsistema_activo = true;
    cargar_sonidos();
    cargar_musica();
}

void Sonido::actualizar() {
    int dt = SDL_GetTicks() - last_t_ms;
    for (auto it = timeouts.begin(); it != timeouts.end();) {
        int& ms = it->second;

        ms -= dt;

        if (ms <= 0)
            it = timeouts.erase(it);
        else
            ++it;
    }
    last_t_ms = SDL_GetTicks();
}

void Sonido::cargar(sonido_t sonido, const char* ruta) {
    if (!subsistema_activo)
        return;
    if (sonidos.find(sonido) != sonidos.end()) {
        log_advertencia("Se intentó cargar dos veces un sonido");
        return;
    }
    
    Mix_Chunk* nuevo_sonido = Mix_LoadWAV(ruta);
    if (!nuevo_sonido) {
        log_advertencia("No se pudo cargar: %s, error: %s", ruta, 
            Mix_GetError());
        return;
    }
    sonidos.emplace(sonido, nuevo_sonido);
}

void Sonido::cargar_sonidos() {
    if (!subsistema_activo)
        return;
    cargar(SND_BASE_BAJO_ATAQUE,
        RUTA_SONIDOS "/anunciador_base_under_attack.ogg");
    cargar(SND_EDIFICIO_PERDIDO,
        RUTA_SONIDOS "/anunciador_building_lost.ogg");
    cargar(SND_CONSTRUYENDO,
        RUTA_SONIDOS "/anunciador_building.ogg");
    cargar(SND_CONSTRUCCION_TERMINADA,
        RUTA_SONIDOS "/anunciador_construction_complete.ogg");
    cargar(SND_ENTRENANDO,
        RUTA_SONIDOS "/anunciador_training.ogg");
    cargar(SND_UNIDAD_LISTA,
        RUTA_SONIDOS "/anunciador_unit_ready.ogg");
    cargar(SND_UNIDAD_PERDIDA, 
        RUTA_SONIDOS "/anunciador_unit_lost.ogg");
    
    cargar(SND_UNIDAD_MARCADA, 
        RUTA_SONIDOS "/harkonnen_il_yes_sir.ogg");
    cargar(SND_UNIDAD_EN_CAMINO, 
        RUTA_SONIDOS "/ordos_ip_on_our_way.ogg");
    cargar(SND_UNIDAD_ATACAR, 
        RUTA_SONIDOS "/harkonnen_il_over_n_out.ogg");
}

void Sonido::habilitar_sonidos(bool activar) {
    sonido_habilitado = activar;
}

bool Sonido::sonido_activo() const {
    return subsistema_activo && sonido_habilitado;
}

void Sonido::reproducir_sonido_(sonido_t sonido) {
    if (!subsistema_activo || !sonido_habilitado)
        return;

    auto it_timeout = timeouts.find(sonido);
    if (it_timeout != timeouts.end())
        return; // Ya se está reproduciendo

    auto it_chunk = sonidos.find(sonido);
    if (it_chunk == sonidos.end())
        return; // No hay sonido -.-
    
    Mix_Chunk *chunk = it_chunk->second;
    if (Mix_PlayChannel(-1, chunk, 0) == -1)
        log_advertencia("No se pudo reproducir: %s", Mix_GetError());
    
    timeouts[sonido] = (chunk->alen * MS_POR_SEG) / FORMATO_AUDIO;
}

void Sonido::cargar_musica() {
    if (!subsistema_activo || musica_fondo)
        return;
    
    const char *ruta = RUTA_SONIDOS "/this_sick_beat.mp3";
    musica_fondo = Mix_LoadMUS(ruta);
    if (!musica_fondo) {
        log_error("No se pudo cargar %s, error: %s", ruta, Mix_GetError());
        return;
    }
}

void Sonido::iniciar_musica_fondo() {
    if (!subsistema_activo || !musica_fondo)
        return;

    if (Mix_PlayingMusic() != 0)
        detener_musica_fondo();
    
    if (Mix_PausedMusic() != 0)
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(musica_fondo, -1);
    
    musica_habilitada = true;
}

bool Sonido::musica_activa() const {
    return subsistema_activo && musica_habilitada;
}

void Sonido::detener_musica_fondo() {
    if (!subsistema_activo)
        return;
    Mix_PauseMusic();
    musica_habilitada = false;
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
