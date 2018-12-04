#ifndef _EDIFICIO_BASE_H_
#define _EDIFICIO_BASE_H_

#include <string>
#include <utility>

#include "libs/json.hpp"

namespace modelo { class EdificioBase; }

namespace modelo {
/**
 * \brief EdificioBase. 
 * 
 * Encampsula la infromacion de un tipo edificio.
 * Tipo: nombre del edificio.
 * Energia: energia consumia por cada unidad del tipo de edificio.
 * Costo: costo (en plata) de construccion.
 * Dimensiones: cantidad de celdas(del terreno) que ocupa cada isntancia.
 * esta en formato alto x ancho.
 * Puntos_de_estructura: cantidad de "vida" que tiene cada instancia.
 */
class EdificioBase{
private:
    std::string tipo;
    unsigned int energia;
    unsigned int costo;
    std::pair<int,int> dimensiones;
    unsigned int puntos_de_estructura;
    
public:
    EdificioBase();
    /**
     * \brief Constructor.
     * crea el edificio de acuerdo a data_edificio
     * en formato JSON.
     */
    explicit EdificioBase(const nlohmann::json& data_edificio);
    ~EdificioBase();
    /**
     * \brief devuelve el tiempo de construccion del edificio.
     */
    unsigned int get_tiempo();
    unsigned int get_energia() const;
    unsigned int get_costo() const;
    unsigned int get_ptos_est();
    std::pair<int,int>& get_dimensiones();
    std::string& get_tipo();
};

} // namespace modelo

#endif // _EDIFICIO_BASE_H_
