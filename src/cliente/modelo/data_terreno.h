#ifndef DATA_TERRENO_H
#define DATA_TERRENO_H

#include <string>
#include <vector>

struct DataTerreno {
    std::string id; // nombre del terreno. por ej: "roca1", "roca2"
    int tipo;
    std::vector<uint32_t> pos_tiles;
};

#endif // DATA_TERRENO_H