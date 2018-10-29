#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <vector>

#include "cliente/json.hpp"

#include "cliente/modelo/celda.h"
#include "cliente/modelo/terreno.h"
#include "cliente/modelo/sprite.h"
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

private:
    std::string id;
    std::string nombre;
    Sprite sprite_base, sprite_construido, sprite_destruido;
    
    int ancho, alto;
    std::vector<Celda> celdas_ocupadas;
    int vida;

    int pos_x, pos_y;
    
};

} // namespace cliente

#endif // _EDIFICIO_H_
