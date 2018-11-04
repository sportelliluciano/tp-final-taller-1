#include "arma.h"

#include "arma_base.h"
#include "arma_caracteristicas.h"
#include "unidad.h"
#include "edificio.h"

Arma(ArmaBase& info_base,ArmaCaracteristicas& caracteristicas):
                            info_base(info_base),
                            caracteristicas(caracteristicas){}
~Arma(){}
void atacar_a(Unidad& victima){

}
void atacar_a(Edificio& victima){

}
