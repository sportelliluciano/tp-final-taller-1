#ifndef _HILO_EMISOR_H_
#define _HILO_EMISOR_H_

#include <iostream>

#include "../src/libs/json.hpp"

#include "../src/conexion/conexion.h"
#include "cola.h"

class HiloEmisor {
public:
    HiloEmisor(conexion::Conexion& conexion_, Cola& cola_) 
    : conexion(conexion_), cola(cola_)
    { }

    void operator()() {
        try {
            while (conexion.esta_conectada()) {
                if (!cola.esta_vacia()) {
                    nlohmann::json data = cola.pull();
                    std::cout << COLOR_ROJO "<< " COLOR_RESET << data.dump() << std::endl;
                    conexion.enviar_json(data);
                }
            }
        } catch(const std::exception& e) {
            std::cout << "HE: conexion perdida (" << e.what() << ")" << std::endl;
        }
        terminar = true;
    }

    bool termino() const {
        return terminar;
    }

private:
    conexion::Conexion& conexion;
    Cola& cola;
    bool terminar = false;
};

#endif // _HILO_EMISOR_H_
