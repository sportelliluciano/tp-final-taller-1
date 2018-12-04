#ifndef _EJERCITO_CREADOR_H_
#define _EJERCITO_CREADOR_H_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>

#include "libs/json.hpp"
#include "modelo/unidad.h"
#include "modelo/terreno.h"
#include "modelo/arma_creador.h"
#include "modelo/cosechadora.h"
#include "modelo/jugador.h"

namespace modelo {
/**
 * \brief EjercitoCreador. 
 * Se encarga de crear todo tipo de unidades, para ello se basa en los 
 * prototipos inicializados con el metodo inicializar.
 * Terreno: puntero al terreno(unica instancia) del juego.
 * Armamento: instancia de ArmaCreador, necesaria para relcionar a las 
 * unidades con las armas que le corresponden.
 * Prototipos_base: mapa donde se guarda una uncia instancia de cada 
 * tipo diferente de UnidadBase.
 */
class EjercitoCreador{
    private:
    Terreno* terreno;
    ArmaCreador armamento;
    std::unordered_map<std::string,UnidadBase> prototipos_base;

    public:
    EjercitoCreador();
    /**
     * \brief Inicializador.
     *  
     * Inicializa todos los prototipos a partir de ejercito, parametro en formato JSON.
     */
    void inicializar(Terreno* terreno_,const nlohmann::json& ejercito);
    ~EjercitoCreador();
    /**
     * \brief Crea un instancia de Unidad especifica
     * basandose en la informacion recibida por parametro.
     */
    Unidad clonar(const std::string& id_tipo,int id,int x,int y);
    /**
     * \brief Crea un instancia de Cosechadora especifica
     * basandose en la informacion recibida por parametro.
     */
    Cosechadora clonar(const std::string& id_tipo,int id,int x,int y,
                        Terreno* terreno_,int id_propietario,
                        Jugador* comunicacion_jugador);
    unsigned int get_costo(const std::string& id_tipo);
    unsigned int get_vida(const std::string& id_tipo);
    unsigned int get_tiempo(const std::string& id_tipo);
    std::pair<int,int>& get_dimensiones(const std::string& id_tipo);
    void get_tiempos_de_entrenamiento(std::unordered_map<std::string,int>& tiempos);
    std::set<std::string>& get_requisitos(const std::string& clase);
    std::set<std::string>& get_casas(const std::string& clase);
};

} // namespace modelo

#endif //_EJERCITO_CREADOR_H_
