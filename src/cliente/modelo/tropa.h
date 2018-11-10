#ifndef _TROPA_H_
#define _TROPA_H_

#include <vector>
#include <utility>

#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Tropa {
public:
    Tropa(int id_tropa_, int x_, int y_);

    /**
     * \brief Renderiza la tropa.
     */
    void renderizar(Ventana& ventana, int x_px, int y_px);

    /**
     * \brief Actualiza el estado de la tropa actual, moviéndola si es 
     *        necesario.
     */
    void actualizar(int t_ms);

    /**
     * \brief Devuelve el ID de tropa.
     */
    int obtener_id() const;
    
    /**
     * \brief Devuelve la coordenada x de la posición de la tropa en 
     *        píxeles globales.
     */
    int obtener_x() const;

    /**
     * \brief Devuelve la coordenada y de la posición de la tropa en 
     *        píxeles globales.
     */
    int obtener_y() const;

    /**
     * \brief Devuelve true si la tropa se está moviendo.
     */
    bool esta_moviendo() const;

    /**
     * \brief Indica a la tropa que camine hacia la posición
     *        (x_dest, y_dest) dada en píxeles globales.
     */
    void caminar_hacia(int x_dest, int y_dest);

    /**
     * \brief Indica a la tropa que siga un camino.
     * 
     * Al indicar un camino la tropa iniciará su recorrido desde el paso 0
     * (que debería ser su posición actual) hacia el paso 1, interpolando
     * linealmente la posición entre los puntos.
     * En el paso 1 se quedará detenida hasta recibir un sync_camino.
     */
    void seguir_camino(const std::vector<std::pair<int, int>>& camino);

    /**
     * \brief Indica a la tropa la posición en la que debería estar parada
     *        respecto al camino a seguir.
     * 
     * Este método es un sincronizmo entre el servidor y el cliente para indicar
     * que la tropa efectivamente está en la posición que debería, y corregirlo 
     * en caso de ser necesario.
     * 
     * Recibido este comando la tropa continúa el camino establecido 
     * previamente.
     */
    void sync_camino(int x_, int y_);

private:
    static const int N_SPRITES = 32;
    SpriteAnimado sprites[N_SPRITES];

    float x, y;
    float x_destino, y_destino;

    int id_tropa = -1;

    std::vector<std::pair<int, int>> camino_actual;
    int paso_actual = 0;
};

} // namespace cliente

#endif // _TROPA_H_
