#ifndef _SONIDO_H_
#define _SONIDO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <unordered_map>

#define MAX_VOLUMEN 100

namespace cliente {

typedef enum {
    SONIDO_BLEEP
} sonido_t;

/**
 * \brief Encapsulamiento del módulo de sonido de SDL.
 * 
 * Permite reproducir sonidos y música de fondo.
 */
class Sonido {
public:
    static Sonido& obtener_instancia();

    /**
     * \brief Encola un sonido a ser reproducido.
     * 
     * Encola un sonido a ser reproducido. El mismo podría no reproducirse
     * si se detecta que hay demasiados sonidos similares sonando.
     */
    void reproducir_sonido(sonido_t sonido, int volumen = 100);

    /**
     * \brief Inicia la reproducción de la música de fondo.
     */
    void iniciar_musica_fondo();

    /**
     * \brief Detiene la reproducción de la música de fondo.
     */
    void detener_musica_fondo();

    /**
     * \brief Configura el volumen de la música de fondo. 
     * 
     * El volumen debe estar entre 0 y 100, siendo 0 silenciado y 100 volumen
     * máximo.
     */
    void set_volumen_musica_fondo(int volumen);

    /**
     * \brief Configura el volumen de los efectos de sonido.
     * 
     * El volumen debe estar entre 0 y 100, siendo 0 silenciado y 100 volumen
     * máximo.
     */
    void set_volumen_sonidos(int volumen);

    /**
     * \brief Apaga el sistema de sonido del juego liberando los recursos.
     */
    void apagar();
    
    /**
     * \brief Libera los recursos asociados.
     */
    ~Sonido();

private:
    std::unordered_map<sonido_t, Mix_Chunk*> sonidos;
    Mix_Music* musica_fondo = NULL;

    float volumen_sonidos = 1.0f;

    bool sonido_habilitado = true;

    /**
     * \brief Inicializa el subsistema de sonido.
     */
    Sonido();
};

} // namespace cliente

#endif // _SONIDO_H_