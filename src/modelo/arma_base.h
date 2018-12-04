#ifndef _ARMA_BASE_H_
#define _ARMA_BASE_H_

#include <string>

#include "libs/json.hpp"

namespace modelo { class ArmaBase; }

#include "modelo/unidad.h"
#include "modelo/edificio.h"
#include "modelo/atacable.h"

namespace modelo {
/**
 * \brief ArmaBase
 * 
 * El arma base es un arma generica que guarda toda la informacion 
 * de cualquier arma.
 * Entre la informacion que posee se encuenta:
 * Tipo: nombre del arma.
 * Dano: cantidad de vida que le saca al oponenete al atacar.
 * Frecuencia: frecuencia de disparo.
 */
class ArmaBase {
private:
    std::string tipo;
    unsigned int dano;
    unsigned int frecuencia;
    //bonificaciones
    
public:
    ArmaBase();
    /**
     * \brief Constructor.
     *  
     * Inicializa el arma a partir de data_arma, atributo en formato JSON.
     */
    explicit ArmaBase(const nlohmann::json& data_arma);
    ~ArmaBase();
    /**
     * \brief Reduce la vida a la victima de acuerdo al atributo dano.
     */
    int atacar_a(Atacable* victima);
    unsigned int obtener_frecuencia();
};

} // namespace modelo

#endif // _ARMA_BASE_H_
