#ifndef _TROPA_H_
#define _TROPA_H_

#include <string>
#include <utility>
#include <vector>

#include "libs/json.hpp"

#include "cliente/modelo/sprite_animado.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Tropa {
public:
    Tropa(const nlohmann::json& data);

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
     * \brief Devuelve la clase a la que pertenece la tropa
     */
    const std::string& obtener_clase() const;

    /**
     * \brief Devuelve el identificador del botón para entrenar la tropa.
     */
    int obtener_sprite_boton() const;
    
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
     * \brief Devuelve true si la tropa está disparando.
     */
    bool esta_disparando() const;

    /**
     * \brief Inicializa la instancia de la tropa.
     */
    void inicializar(int id, const Posicion& posicion, int vida_, 
        int id_propietario_);

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

    /**
     * \brief Setea la vida de la tropa.
     */
    void set_vida(int nueva_vida);
    
    /**
     * \brief Devuelve el ID del jugador a quien pertenece esta tropa.
     */
    int obtener_propietario() const;

    void set_esta_disparando(bool disparando);

    /**
     * \brief Agrega una marca de selección a la tropa.
     */
    void marcar();

    /**
     * \brief Elimina la marca de selección de la tropa.
     */
    void desmarcar();

private:
    int x_actual, y_actual;
    float fx_actual, fy_actual;
    int x_destino, y_destino;

    int id_tropa = -1;
    int id_propietario = 0;

    std::vector<std::pair<int, int>> camino_actual;
    size_t paso_actual = 0;
    int vida = 100;

    bool esta_marcada = false;

    std::string clase;

    static const int N_SPRITES = 8;
    SpriteAnimado sprites_caminando[N_SPRITES];
    SpriteAnimado sprites_parado[N_SPRITES];
    SpriteAnimado sprites_disparando[N_SPRITES];
    SpriteAnimado sprites_vehiculo[N_SPRITES*4];
    int posicion_sprite = 0; // Hacia donde está mirando la tropa

    int sprite_boton;

    bool b_esta_disparando = false;

    SpriteAnimado& obtener_sprite();

    bool es_vehiculo = false;
    int nueva_pos_sprite = 0;
    int velocidad = 0;

    int last_ms = 0;
};

} // namespace cliente

#endif // _TROPA_H_
