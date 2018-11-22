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
};

} // namespace cliente

#endif // _TOOLTIP_H_
