#ifndef _ARMA_CREADOR_H_
#define _ARMA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "libs/json.hpp"

#include "modelo/arma_base.h"
//#include "modelo/arma_caracteristicas.h"

namespace modelo {
/**
 * \brief ArmaCreador
 * 
 * Crea todos los prototipos de armas.
 * Cada arma es unica y solo se instancia una vez.
 */
class ArmaCreador{
    private:
    std::unordered_map<std::string,Arma> prototipos;
    std::unordered_map<std::string,ArmaBase> armas_base;
    void inicializar_armas_base(const nlohmann::json& data_armas_base);

    public:
    ArmaCreador();
    /**
     * \brief Constructor.
     *  
     * Inicializa todos los prototipos a partir de data_armas_base, atributo en formato JSON.
     */
    void inicializar(const nlohmann::json& data_armas_base);
    ~ArmaCreador();
    /**
     * \brief Devuelve una referencia al arma con nombre id.
     */
    Arma& get(const std::string& id);
    /**
     * \brief Devuelve true en caso de que el arma este dentro de los prototipos,
     * devuelve false, en caso contrario.
     */
    bool tiene(const std::string& id);
};
}
#endif //_ARMA_CREADOR_H_
