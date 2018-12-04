#ifndef MANEJADOR_JSON_H
#define MANEJADOR_JSON_H

#include <string>
#include <vector>
#include <map>
#include "editor/label_mapa.h"

class ManejadorJson {
    public: 
        /**
         * \brief Constructor ManejadorJson.
         * 
         * Constructor de ManejadorJson.
         */
        ManejadorJson(){};

        /**
         * \brief Genero mapa en formato .json .
         * 
         * Genero el Json con el nombre, las filas, columnas, el mapa y la 
         * posicion de los jugadores en el mapa.
         */
        void generar_json(std::string& nombre_archivo, int filas, int columnas,
            std::map<std::string, LabelMapa*>& mapa, 
            std::map<std::string, bool>& jugadores);

        /**
         * \brief Destructor ManejadorJson.
         * 
         * Destructor de ManejadorJson.
         */
        ~ManejadorJson(){};
        
    private:
        /**
         * \brief Split de string.
         * 
         * Devuelvo un vector de strings a partir del splitteo de un string y un 
         * caracter delimitador.
         */
        std::vector<std::string> split(const std::string& str, char delim);
};

#endif // MANEJADOR_JSON_H