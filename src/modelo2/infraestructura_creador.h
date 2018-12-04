#ifndef _INFRAESTRUCTURA_CREADOR_H_
#define _INFRAESTRUCTURA_CREADOR_H_

#include <unordered_map>
#include <string>

#include "modelo/edificio.h"
#include "modelo/edificio_base.h"

namespace modelo {
/**
 * \brief InfraestructuraCreador. 
 * Se encarga de crear todo tipo de edificio, para ello se basa en los 
 * prototipos inicializados con el metodo inicializar.
 * Prototipos mapa donde se guarda una uncia instancia de cada 
 * tipo diferente de Edificio.
 * info_base: mapa donde se guarda una uncia instancia de cada 
 * tipo diferente de EdificioBase.
 */
class InfraestructuraCreador{
    private:
    std::unordered_map<std::string,Edificio> prototipos;
    std::unordered_map<std::string,EdificioBase> info_base;    
    public:

    InfraestructuraCreador();
    /**
     * \brief Inicializador.
     *  
     * Inicializa todos los prototipos a partir de edificios, parametro en formato JSON.
     */
    void inicializar(const nlohmann::json& edificios);
    ~InfraestructuraCreador();
    /**
     * \brief Crea un instancia de Edifcio especifica
     * basandose en la informacion recibida por parametro.
     */
    Edificio clonar(const std::string& id_tipo,int id,int x,int y);
    unsigned int get_costo(const std::string& id_tipo);
    unsigned int get_energia(const std::string& id_tipo);
    std::pair<int,int>& get_dimensiones(const std::string& id_tipo);
};

} // namespace modelo

#endif // _INFRAESTRUCTURA_CREADOR_H_
