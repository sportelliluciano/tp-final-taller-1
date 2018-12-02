#include "ManejadorJson.h"
#include "libs/json.hpp"
#include <fstream>
#include <sstream>
#include "GeneradorSprites.h"
#define DELIM_ID ','
using std::vector;
using std::map;
using std::string;
using std::stringstream;
using std::getline;
using nlohmann::json;

void ManejadorJson::generar_json(string& nombre_archivo, int filas, int columnas,
    map<string, LabelMapa*>& mapa, map<string, bool>& jugadores) {
    json j;
    
    vector<vector<string>> tipos;
    vector<vector<uint32_t>> jugadores_json;
    int cont_sprites_agregados = 0;

    for (int i = 0; i < filas; ++i) {
        vector<string> tipos_por_columna;
        
        for (int j = 0; j < columnas; ++j) {
            string id_label ("");
            id_label += std::to_string(i);
            id_label += ',';
            id_label += std::to_string(j);
            map<string, LabelMapa*>::iterator it = mapa.find(id_label);
	        if (it != mapa.end()) {
                tipos_por_columna.emplace_back(it->second->get_sprite().id);
            }
        }

        tipos.emplace_back(tipos_por_columna);
    }

    vector<uint32_t> pos_jugadores;
    map<string, bool>::iterator it_jugadores = jugadores.begin();
    for (; it_jugadores != jugadores.end(); ++it_jugadores) {
        vector<string> splitteado = this->split(it_jugadores->first, DELIM_ID);
        pos_jugadores.emplace_back(std::stoi(splitteado[0]));
        pos_jugadores.emplace_back(std::stoi(splitteado[1]));
        jugadores_json.emplace_back(pos_jugadores);
        pos_jugadores.clear();
    }

    // agrego data al json
    j["tipo"] = tipos;
    j["jugadores"] = jugadores_json;

    // genero el archivo mapa.json
    std::ofstream file(nombre_archivo);
    file << j;
}

vector<string> ManejadorJson::split(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> elementos;
    while (std::getline(ss, item, delim)) {
        elementos.emplace_back(item);
    }
    return elementos;
}
