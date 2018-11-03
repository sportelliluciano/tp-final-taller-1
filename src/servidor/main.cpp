#include <iostream>
#include <fstream>

#include "libs/json.hpp"

int main(){

    using nlohmann::json;

    std::ifstream entrada("../data/armas.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        std::cout << elem["nombre"] << '\n';
        std::cout << elem["dano"] << '\n';
    }
    return 0;    
}
