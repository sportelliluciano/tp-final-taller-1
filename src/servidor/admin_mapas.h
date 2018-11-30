#ifndef _ADMIN_MAPAS_H_
#define _ADMIN_MAPAS_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "libs/json.hpp"

namespace servidor {

/**
 * \brief Carga la información sobre los mapas disponibles.
 */
class AdminMapas {
public:
    /**
     * \brief Carga la información de los mapas.
     * 
     * rutas_mapas debe ser un vector de rutas a los mapas disponibles
     */
    AdminMapas(const std::vector<std::string>& rutas_mapas);

    /**
     * \brief Devuelve una lista con el nombre de cada mapa disponible.
     */
    const std::vector<std::string>& obtener_nombres() const;

    /**
     * \brief Devuelve el mapa a partir del nombre del mismo.
     * 
     * nombre debe ser uno de los nombres devueltos por el método 
     * obtener_nombres
     */
    const nlohmann::json& obtener_mapa(const std::string& nombre) const;

    /**
     * \brief Devuelve true si exise un mapa con el nombre indicado.
     */
    bool existe_mapa(const std::string& nombre) const;

    /**
     * \brief Devuelve la cantidad de jugadores que admite el mapa.
     */
    size_t obtener_cantidad_jugadores(const std::string& mapa) const;

private:
    std::unordered_map<std::string, nlohmann::json> mapas;
    std::vector<std::string> nombres;
};

} // namespace servidor

#endif // _ADMIN_MAPAS_H_
