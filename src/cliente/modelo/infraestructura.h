#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <unordered_map>
#include <vector>

#include "cliente/modelo/celda.h"
#include "cliente/modelo/edificio.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Infraestructura {
public:
    Infraestructura(int id_jugador_actual_, Terreno& terreno, 
        const nlohmann::json& edificios);

    /**
     * \brief Renderiza los edificios en la ventana.
     */
    void renderizar(Ventana& ventana, Camara& camara);

    /**
     * \brief Actualiza el estado de las construcciones.
     */
    void actualizar(int t_ms);

    
    /******* Métodos para actualizar el HUD *******/

    bool existe(int id_edificio) const;

    Edificio& obtener(int id_edificio);

    bool hay_edificio_enemigo_en(const Posicion& punto) const;

    Edificio& obtener_edificio_enemigo_en(const Posicion& punto);

    /**
     * \brief Obtiene el centro de construcción del jugador actual.
     */
    Edificio* obtener_centro_construccion();

    /**
     * \brief Devuelve los edificios disponibles para construir.
     */
    const std::vector<const Edificio*>& obtener_edificios_base() const;

    /**
     * \brief Devuelve true si se está construyendo un edificio de la clase
     *        indicada.
     */
    bool esta_construyendo(const std::string& clase) const;

    /**
     * \brief Devuelve la cantidad de edificios que hay en la cola de 
     *        construcción de la clase especificada.
     */
    int obtener_cola_construccion(const std::string& clase) const;

    /**
     * \brief Devuelve la cantidad de segundos restantes para que se construya
     *        el próximo edificio de la clase indicada.
     */
    int obtener_segundos_restantes(const std::string& clase) const;

    /**
     * \brief Devuelve el sprite para el botón asociado a la clase indicada.
     */
    int obtener_sprite_clase(const std::string& clase) const;

    
    bool jugador_actual_tiene(const std::string& clase) const;

    const Edificio& obtener_edificio_base(const std::string& clase) const;

    /******* Métodos para actualizar desde el servidor *******/

    
    /**
     * Inicia la construcción de un edificio.
     */
    void iniciar_construccion(const std::string& clase, int tiempo_ms);

    /**
     * Sincroniza la construcción de un edificio.
     */
    void sincronizar_construccion(const std::string& clase, int tiempo_ms);

    /**
     * Actualiza la cola de construcciones según la cantidad
     */
    void actualizar_cola(const std::string& clase, int cantidad);

    /**
     * Setea la velocidad de construcción.
     * 1.0 es normal, 2.0 es el doble de rápido, 0.5 la mitad de velocidad.
     */
    void set_velocidad_construccion(float velocidad);

    /**
     * Actualiza la vida de un edificio.
     */
    void atacar(int id, int nueva_vida);

    /**
     * Crea un nuevo edificio sobre el terreno.
     */
    void crear_edificio(int id, int jugador, const std::string& clase, 
        const std::vector<int>& posicion);
    
    /**
     * Agrega un edificio que ya existía pero entró en el campo de visión del
     * jugador.
     */
    void agregar_edificio(int id, int jugador, const std::vector<int>& posicion, 
        const std::string& clase, int vida);
    
    /**
     * Elimina un edificio que fue eliminado previamente pero recién ahora
     * el jugador observó la zona.
     */
    void eliminar_edificio(int id);

    /**
     * Destruye un edificio sobre el terreno.
     */
    void destruir_edificio(int id);

private:
    Terreno& terreno;
    std::unordered_map<int, Edificio> edificios_construidos;

    std::unordered_map<std::string, Edificio> edificios;

    std::unordered_map<std::string, int> construcciones_iniciadas;
    std::unordered_map<std::string, int> colas_construccion;

    std::vector<const Edificio*> edificios_base_ordenados;

    float velocidad_cc = 1.0f;

    int last_ms = -1;

    int id_jugador_actual;
};

} // namespace cliente

#endif // _INFRAESTRUCTURA_H_
