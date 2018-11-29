#ifndef _EJERCITO_CREADOR_H_
#define _EJERCITO_CREADOR_H_

#include <string>
#include <unordered_map>  

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "modelo/terreno.h"
#include "modelo/arma_creador.h"
#include "modelo/cosechadora.h"
#include "modelo/jugador.h"

namespace modelo {

class EjercitoCreador{
    private:
    Terreno* terreno;
    ArmaCreador armamento;
    std::unordered_map<std::string,UnidadBase> prototipos_base;

    public:
    EjercitoCreador();
    void inicializar(Terreno* terreno_,const nlohmann::json& ejercito);
    ~EjercitoCreador();
    Unidad clonar(std::string id_tipo,int id,int x,int y);
    Cosechadora clonar(std::string id_tipo,int id,int x,int y,
                        Terreno* terreno_,int id_propietario,
                        Jugador* comunicacion_jugador);
    unsigned int get_costo(std::string id_tipo);
    unsigned int get_vida(std::string id_tipo);
    unsigned int get_tiempo(std::string id_tipo);
    std::pair<int,int>& get_dimensiones(std::string id_tipo);
    void get_tiempos_de_entrenamiento(std::unordered_map<std::string,int>& tiempos);
};
}
#endif
