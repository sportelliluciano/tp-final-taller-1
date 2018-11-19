#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "conexion/i_modelo.h"
#include "conexion/i_jugador.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"

namespace modelo {

class Jugador{
    private:
    std::string casa;
    std::set<int> inventario;
    int energia = 100000;
    unsigned int energia_max = 10000000;
    unsigned int consumo = 0;
    IJugador* comunicacion_jugador;
    std::unordered_map<std::string, int> construcciones;
    std::unordered_set<std::string> construcciones_esperando_ubicacion;
    std::unordered_map<std::string, int> construcciones_en_cola;

    std::unordered_map<std::string, int> tropas;
    std::unordered_map<std::string, int> tropas_en_cola;

    public:
    Jugador(std::string casa,IJugador* jugador);
    ~Jugador();
    void aumentar_consumo(unsigned int consumo_);
    void reducir_consumo(unsigned int consumo_);
    void aumentar_energia(unsigned int energia_);
    void reducir_energia(unsigned int energia_);
    std::string get_casa();
    bool hay_suficiente_energia(unsigned int costo);
    bool empezar_construccion(const std::string& clase,unsigned int costo);
    bool empezar_entrenamiento(const std::string& clase,unsigned int costo);
    bool cancelar_construccion(const std::string& clase,unsigned int costo);
    bool cancelar_entrenamiento(const std::string& clase,unsigned int costo);
    void agregar_elemento(int id,unsigned int energia_,const std::string& clase);
    void eliminar_elemento(int id,unsigned int energia_consumida);
    bool pertenece(int id);
    void actualizar_construcciones(int dt,Infraestructura& inf);
    std::string actualizar_tropas(int dt,Ejercito& ejercito);
    IJugador* get_jugador();
};
}
#endif
