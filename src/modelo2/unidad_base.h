#ifndef _UNIDAD_BASE_H_
#define _UNIDAD_BASE_H_

#include <string>
#include <utility>
#include <set>

#include "libs/json.hpp"

namespace modelo { class UnidadBase; }
#include "modelo/arma.h"

namespace modelo {

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
    UnidadBase(const nlohmann::json& data_unidad, const std::vector<Arma*>& armas_);
    ~UnidadBase();
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
