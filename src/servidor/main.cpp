#include <iostream>
#include <fstream>

#include "libs/json.hpp"

int main(){

    using nlohmann::json;

    std::ifstream entrada("../data/edificios.json");

    json edificios_json;

    entrada >> edificios_json;

    auto it = edificios_json.begin();
    const json& valores_por_defecto = *it;
    ++it;
    for(; it != edificios_json.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        json elem = valores_por_defecto;
        elem.update(*it);

        std::cout << elem["id"] << '\n';
        std::cout << elem["nombre"] << '\n';
        std::cout << elem["energia"] << '\n';
        std::cout << elem["costo"] << '\n';
        std::cout << elem["puntos_estructura"] << '\n';
    }
    return 0;    
}
