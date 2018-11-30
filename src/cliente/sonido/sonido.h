#ifndef _SONIDO_H_
#define _SONIDO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <unordered_map>

#define MAX_VOLUMEN 100

namespace cliente {

typedef enum {
    SND_BASE_BAJO_ATAQUE,
    SND_EDIFICIO_PERDIDO,
    SND_CONSTRUYENDO,
    SND_CONSTRUCCION_TERMINADA,
    SND_ENTRENANDO,
    SND_UNIDAD_LISTA,
    SND_UNIDAD_PERDIDA
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
    static void reproducir_sonido(sonido_t sonido);

    /**
     * \brief Actualiza las colas de reproducción.
     */
    void actualizar();

    /**
     * \brief Inicia la reproducción de la música de fondo.
     */
    void iniciar_musica_fondo();

    /**
     * \brief Detiene la reproducción de la música de fondo.
     */
    void detener_musica_fondo();

    /**
     * \brief Activa o desactiva los efectos de sonido.
     * 
     */
    void habilitar_sonidos(bool activar);

    /**
     * \brief Devuelve true si el sonido está habilitado.
     */
    bool sonido_activo() const;

    /**
     * \brief Devuelve true si la música está activa actualmente.
     */
    bool musica_activa() const;

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
    std::unordered_map<sonido_t, int> timeouts;
    Mix_Music* musica_fondo = NULL;

    bool sonido_habilitado = true;
    bool musica_habilitada = false;
    bool subsistema_activo = false;

    int last_t_ms = 0;

    /**
     * \brief Inicializa el subsistema de sonido.
     */
    Sonido();

    void reproducir_sonido_(sonido_t sonido);
    void cargar_sonidos();
    void cargar_musica();
    void cargar(sonido_t sonido, const char* ruta);
};

} // namespace cliente

#endif // _SONIDO_H_