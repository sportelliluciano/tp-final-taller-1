#ifndef _TOOLTIP_H_
#define _TOOLTIP_H_

#include <string>
#include <vector>
#include <utility>

#include "cliente/video/ventana.h"

namespace cliente {

class Tooltip {
public:
    void set_titulo(const std::string& titulo);
    void set_cuerpo(const std::string& cuerpo);
    void set_metadata(
        const std::vector<std::pair<std::string, std::string>>& pie);
    
    void set_costo_tiempo(int costo, float tiempo);
    void renderizar(Ventana& ventana, int x, int y);

private:
    std::string titulo_, cuerpo_;
    std::string costo_, tiempo_;
    std::vector<std::string> meta;

    std::string id_textura;

    /**
     * \brief Renderiza los metadatos sobre la textura indicada.
     * 
     * tooltip: Textura sobre la que se renderizaran los metadatos
     * offset_y: Offset vertical para dibujar el primer metadato
     * encuadre: Encuadre para wrappear el texto si es necesario.
     */
    void renderizar_metadata(AdministradorTexturas& admin_texturas, 
        Textura& tooltip, int offset_y, const Rectangulo& encuadre);
    
    /**
     * \brief Crea la textura con el texto del cuerpo renderizado.
     * 
     * La fuente de la misma se ajustará según corresponda para que entre en
     * el encuadre.
     */
    Textura crear_textura_cuerpo(AdministradorTexturas& admin_texturas,
        const Rectangulo& encuadre);
    
    /**
     * \brief Crea el tooltip.
     * 
     * Realiza el renderizado del tooltip sobre una nueva textura y lo
     * devuelve.
     * 
     * La nueva textura se almacenará en la caché de la ventana.
     */
    Textura& crear_tooltip(AdministradorTexturas& admin_texturas);

    /**
     * \brief Obtiene la textura del tooltip desde la caché de la ventana
     *        si es que ya fue renderizada o la crea.
     * 
     */
    Textura& obtener_textura(Ventana& ventana);
};

} // namespace cliente

#endif // _TOOLTIP_H_
