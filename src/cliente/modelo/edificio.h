#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <vector>

#include "libs/json.hpp"

#include "cliente/modelo/celda.h"
#include "cliente/modelo/terreno.h"
#include "cliente/modelo/sprite_animado.h"
#include "cliente/modelo/sprite_compuesto.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Edificio del juego.
 */
class Edificio {
public:
    /**
     * \brief Constructor.
     * 
     * TODO: Sí, sé que esto es horrible, pero hasta que no quede definida la
     * interfaz, prefiero mantenerlo de esta forma.
     */
    Edificio(const nlohmann::json& data_edificio);
    /**
     * \brief Devuelve las celdas que ocupa el edificio
     */
    const std::vector<Celda>& obtener_celdas_ocupadas() const;

    /**
     * \brief Renderiza el edificio.
     */
    void renderizar(const Terreno& terreno, Ventana& ventana);

    /**
     * \brief Construye el edificio sobre el terreno en la posición (x, y).
     */
    void construir(const Terreno& terreno, int x, int y);

    /**
     * \brief Destruye el edificio.
     * 
     * Muestra la animación de destrucción del edificio.
     */
    void destruir();

    /**
     * \brief Devuelve false si el edificio fue destruido y su animación 
     *        terminó.
     */
    bool esta_vivo() const;

    /**
     * \brief Devuelve el número de sprite que corresponde al botón del HUD.
     */
    int obtener_sprite_boton() const;

    /**
     * \brief Devuelve el ID de edificio
     */
    const std::string& obtener_id() const;

private:
    std::string id;
    std::string nombre;
    
    int ancho, alto;
    std::vector<Celda> celdas_ocupadas;
    int vida;

    int pos_x, pos_y;
    
    int sprite_boton;

    bool esta_destruido = false;
    bool esta_construido = false;

    SpriteAnimado sprite_construccion, sprite_destruccion;
    SpriteCompuesto sprite, sprite_roto;
};

} // namespace cliente

#endif // _EDIFICIO_H_
