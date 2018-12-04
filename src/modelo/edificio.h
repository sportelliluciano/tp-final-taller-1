#ifndef _EDIFICIO_H_
#define _EDIFICIO_H_

#include <string>
#include <utility>

#include "modelo/edificio_base.h"
#include "modelo/edificio_caracteristicas.h"
#include "modelo/atacable.h"

namespace modelo {
/**
 * \brief Edificio. 
 * 
 * Caracteristicas: informacion especifica de la instancia.
 * Info_base: informacion general del tipo de edificio.
 */
class Edificio :public Atacable{
    private:
    EdificioCaracteristicas caracteristicas;
    EdificioBase& info_base;

    public:
    /**
     * \brief Constructor de prototipos de edificio.
     */
    explicit Edificio(EdificioBase& info_base);
    Edificio(EdificioBase& info_base,int id,int x,int y);
    ~Edificio();
    Edificio& operator=(const Edificio& other);
    /**
     * \brief Crea un instancia de edificio especifica
     * basandose en la informacion recibida por parametro.
     */
    Edificio clonar(int id,int x,int y);
    /**
     * \brief le resta dano a puntos_de_estructura.
     * devuelve los puntos_de_estructura actualizados.
     */
    int recibir_dano(unsigned int dano);
    
    int get_id();
    int get_vida();//temporal
    unsigned int get_costo();
    unsigned int get_energia();
    std::pair<int,int>& get_dimensiones();
    Posicion& get_posicion();
    unsigned int get_tiempo();
    std::string& get_tipo() const;
};

} // namespace modelo

#endif //_EDIFICIO_H_
