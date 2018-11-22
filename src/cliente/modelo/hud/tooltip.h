#ifndef _TOOLTIP_H_
#define _TOOLTIP_H_

#include <string>

#include "cliente/video/ventana.h"

namespace cliente {

class Tooltip {
public:
    void set_titulo(const std::string& titulo);
    void set_cuerpo(const std::string& cuerpo);
    void set_pie(const std::string& pie);
    void renderizar(Ventana& ventana, int x, int y);

private:
    std::string titulo_, cuerpo_, pie_;
};

} // namespace cliente

#endif // _TOOLTIP_H_
