#include "modelo/arma.h"

#include "modelo/arma_base.h"
#include "modelo/arma_caracteristicas.h"
#include "modelo/unidad.h"
#include "modelo/edificio.h"

namespace modelo {

Arma(ArmaBase& info_base,ArmaCaracteristicas& caracteristicas):
                            info_base(info_base),
                            caracteristicas(caracteristicas){}
~Arma(){}
void atacar_a(Unidad& victima){

}
void atacar_a(Edificio& victima){

}
}
