#ifndef _TROPA_H_
#define _TROPA_H_

#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

#include "libs/json.hpp"

#include "cliente/modelo/sprite_animado.h"
#include "cliente/modelo/hud/barra_vida.h"
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
    
    

    void set_esta_disparando(bool disparando);

    /**
     * \brief Agrega una marca de selección a la tropa.
     */
    void marcar();

    /**
     * \brief Elimina la marca de selección de la tropa.
     */
    void desmarcar();

    /**
     * \brief Devuelve el ID del jugador a quien pertenece esta tropa.
     */
    int obtener_propietario() const;

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
     * \brief Devuelve un vector con las clases de edificios requeridas para
     *        poder entrenar esta tropa.
     */
    const std::vector<std::string>& obtener_requerimientos() const;

    /**
     * \brief Devuelve true si la casa indicada puede entrenar esta tropa, 
     *        false en caso contrario.
     */
    bool casa_puede_entrenar(const std::string& casa) const;

    /**
     * \brief Devuelve el nombre de la tropa.
     */
    const std::string& obtener_nombre() const;

    /**
     * \brief Devuelve la descripción de la tropa.
     */
    const std::string& obtener_descripcion() const;

    /**
     * \brief Devuelve datos **extra** sobre la tropa, para mostrar en su
     *        mensaje descriptivo
     */
    const std::vector<std::pair<std::string, std::string>>& 
        obtener_metadata() const;
    
    /**
     * \brief Devuelve el costo para el entrenamiento de la tropa.
     */
    int obtener_costo() const;

    /**
     * \brief Devuelve el tiempo, en minutos, requerido para entrenar la tropa.
     */
    float obtener_tiempo_entrenamiento() const;

private:
    int x_actual, y_actual;
    float fx_actual, fy_actual;
    int x_destino, y_destino;

    int id_tropa = -1;
    int id_propietario = 0;

    std::vector<std::pair<int, int>> camino_actual;
    size_t paso_actual = 0;
    int vida = 100, vida_maxima;

    bool esta_marcada = false;

    std::string clase;

    static const int N_SPRITES = 8;
    SpriteAnimado sprites_caminando[N_SPRITES];
    SpriteAnimado sprites_parado[N_SPRITES];
    SpriteAnimado sprites_disparando[N_SPRITES];
    SpriteAnimado sprites_falleciendo[N_SPRITES];

    static const int N_SPRITES_VEHICULO = 32;
    SpriteAnimado sprites_vehiculo[N_SPRITES_VEHICULO];
    SpriteAnimado sprite_disparo;
    SpriteAnimado sprite_descarga;

    //Disparo* disparo = nullptr;
    
    /**
     * \brief Orientación de la tropa según hacia donde mire la tropa.
     */
    int orientacion_sprite = 0;
    int nueva_orientacion_sprite = 0;

    /**
     * \brief Sprite para el boton de entrenamiento
     */
    int sprite_boton;

    /**
     * \brief Si esta variable es true entonces la tropa está disparando
     */
    bool b_esta_disparando = false;


    bool es_vehiculo = false;
    
    int velocidad = 0;

    int last_ms = 0;
    BarraVida barra_vida;

    std::vector<std::string> requerimientos;
    std::unordered_set<std::string> casas_habilitadas;
    std::string nombre, descripcion;
    std::vector<std::pair<std::string, std::string>> metadata;
    int costo;
    float tiempo_entrenamiento;

    void cargar_sprites_tropa(int sprite_base);
    void cargar_sprites_vehiculo(int sprite_base);
    SpriteAnimado& obtener_sprite();
};

} // namespace cliente

#endif // _TROPA_H_
