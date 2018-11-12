#ifndef _SPRITE_ANIMADO_H_
#define _SPRITE_ANIMADO_H_

#include <vector>

#include "cliente/modelo/sprite.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Sprite animado.
 */
class SpriteAnimado {
public:
    /**
     * \brief Crea un nuevo sprite animado vacío.
     * 
     * Llamar a renderizar sobre este sprite no tienen ningún efecto.
     */
    SpriteAnimado() = default;

    // SpriteAnimado(const nlohmann::json& sprite);

    /**
     * \brief Crea un nuevo sprite animado tomando sprite_inicial como el primer
     *        cuadro y sprite_final como el último.
     * 
     * El sprite iniciará en el primer cuadro mostrará consecutivamente los 
     * sprites siguientes hasta llegar al sprite final. La animación se 
     * reproducirá al frame rate esperado.
     * 
     * Podrían saltearse cuadros si el frame rate al que se ejecuta el juego es
     * menor al frame rate esperado.
     */
    SpriteAnimado(int sprite_inicial, int sprite_final, int frame_rate = 30);

    /**
     * \brief Crea un nuevo sprite animado a partir de los cuadros indicados.
     * 
     * El sprite iniciará en el primer cuadro y se reproducirá según el frame
     * rate indicado.
     * 
     * Podrían saltearse cuadros si el frame rate al que se ejecuta el juego es
     * menor al frame rate esperado.
     */
    SpriteAnimado(const std::vector<Sprite>& cuadros_, int frame_rate = 30);

    /**
     * \brief Configura el desplazamiento respecto de la posición de un cuadro.
     * 
     * Permite modificar el desplazamiento de un cuadro respecto de la posición
     * de dibujado.
     * 
     * Lanza runtime_error si el cuadro no existe.
     */
    void desplazar_cuadro(int id_cuadro, int dx, int dy);

    /**
     * \brief Renderiza el cuadro actual en la posición (x, y).
     * 
     * Renderiza el cuadro actual de la animación en la posición (x, y).
     * Si la animación terminó no hace nada.
     */
    void renderizar(Ventana& ventana, int x, int y);

    /**
     * \brief Configura la repetición de la animación.
     * 
     * Si continuar_indefinidamente es true entonces la animación se reiniciará
     * al primer cuadro al finalizar. En caso contrario se detendrá al finalizar
     * el renderizado del último cuadro.
     */
    void configurar_repeticion(bool continuar_indefinidamente);

    /**
     * \brief Reinicia la animación desde el primer cuadro.
     */
    void reiniciar();

    /**
     * \brief Centra la animación alrededor del punto de renderizado.
     */
    void set_centrado(bool habilitar);

    /**
     * \brief Devuelve true si la animación finalizó
     */
    bool finalizado() const;

private:
    std::vector<Sprite> cuadros;
    unsigned int cuadro_actual = 0, ms_por_cuadro, ts_ultimo_cuadro = 0;
    bool loop = false, terminado = true, centrar = false;
};

} // namespace cliente

#endif // _SPRITE_ANIMADO_H_
