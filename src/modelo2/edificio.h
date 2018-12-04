#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

//namespace modelo { class Edificio; }
#include "modelo/edificio_base.h"
#include "modelo/edificio_caracteristicas.h"
#include "modelo/atacable.h"

namespace modelo {

class Edificio :public Atacable{
    private:
    EdificioCaracteristicas caracteristicas;
    EdificioBase& info_base;

    public:
    explicit Edificio(EdificioBase& info_base);
    Edificio(EdificioBase& info_base,int id,int x,int y);
    ~Edificio();
    Edificio& operator=(const Edificio& other);
    Edificio clonar(int id,int x,int y);
    int recibir_dano(unsigned int dano);
    int get_id();
    int get_vida();//temporal
    unsigned int get_costo();
    unsigned int get_energia();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();
    unsigned int get_tiempo();
    std::string& get_tipo();

};
}
#endif
