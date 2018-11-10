#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <vector>

#include "libs/json.hpp"

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
     * \brief Renderiza el edificio en la posición (x, y).
     * 
     * (x, y) está dado en píxeles relativos a la ventana.
     */
    void renderizar(Ventana& ventana, int x, int y, bool seleccionado = false);

    /**
     * \brief Construye el edificio en la celda (celda_x, celda_y).
     */
    void construir(int id_edificio_, int celda_x, int celda_y);

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
    int obtener_id() const;

    /**
     * \brief Devuelve la clase de edificio
     */
    const std::string& obtener_clase() const;

    /**
     * \brief Devuelve la coordenada x de la celda superior izquierda del
     *        edificio.
     */
    int obtener_celda_x() const;

    /**
     * \brief Devuelve la coordenada y de la celda superior izquierda del
     *        edificio.
     */
    int obtener_celda_y() const;

    /**
     * \brief Devuelve el ancho del edificio, en celdas.
     */
    int obtener_ancho_celdas() const;

    /**
     * \brief Devuelve el alto del edificio, en celdas.
     */
    int obtener_alto_celdas() const;

private:
    std::string id;
    std::string nombre;
    
    int ancho, alto;
    
    int vida;

    int id_edificio = -1;

    int pos_x, pos_y;
    
    int sprite_boton;

    bool esta_destruido = false;
    bool esta_construido = false;

    SpriteAnimado sprite_construccion, sprite_destruccion;
    SpriteCompuesto sprite, sprite_roto;
};

} // namespace cliente

#endif // _EDIFICIO_H_
