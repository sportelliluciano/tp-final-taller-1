
#ifndef _UNIDAD_BASE_H_
#define _UNIDAD_BASE_H_

#include <string>
#include <utility>
#include <set>
#include <vector>

#include "libs/json.hpp"

namespace modelo { class UnidadBase; }
#include "modelo/arma.h"

namespace modelo {
/**
 * \brief UnidadBase. 
 * 
 * Encampsula la infromacion de un tipo de unidad.
 * Requisitos: Edificios que se deben tener para poder entrenar
 * una unidad de esta clase.
 * Casa: casa a la que pertence la casa. Solo estas casas
 * pueden tener una instancia de esta unidad.
 * Clase: nombre de la unidad.
 * Rango: distancia minima a la que tiene que estar la unidad para poder
 * atacar (medido en celdas).
 * Velocidad: constante utilizada para calcular el largo de cada paso
 * que da la unidad al caminar.
 * Tiempo_de_entrenamiento: tiempo por default que se tarda en entrenar
 * cada unidad de esta clase.  
 * Costo: cantidad de plata necesaria para poder entenar una intancia de 
 * esta unidad.
 * vida: cantidad de vida que posee la unidad. A llegar a cero la unidad es
 * destruida.
 * Armas: armas relacionadas con la unidad.
 */
class UnidadBase {
private: 
    std::set<std::string> casa;
    std::set<std::string> requisitos;
    std::string clase;
    unsigned int rango;
    unsigned int velocidad;
    unsigned int tiempo_de_entrenamiento;
    unsigned int costo;
    unsigned int vida;
    std::vector<Arma*> armas;
    std::pair <int,int> dimensiones = std::pair <int,int> (1,1);

public:
    /**
     * \brief Constructor.
     * crea una unidad de acuerdo a data_unidad
     * en formato JSON.
     */
    UnidadBase(const nlohmann::json& data_unidad, 
                    const std::vector<Arma*>& armas_);
    ~UnidadBase();
    /**
     * \brief Ejecuta el ataque hacia victima.
     */
    int atacar_a(Atacable* victima);
    unsigned int get_rango() const;
    unsigned int get_velocidad() const;
    unsigned int get_tiempo() const;
    unsigned int get_costo() const;
    unsigned int get_vida() const;
    std::pair<int,int>& get_dimensiones();
    std::string& get_clase();
    unsigned int obtener_frecuencia();
    std::set<std::string>& get_requisitos();
    std::set<std::string>& get_casas();
};

} // namespace modelo

#endif // _UNIDAD_BASE_H_
