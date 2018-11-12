#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>
#include <unordered_map>
#include <set>

namespace modelo {

class Jugador{
    private:
    std::string casa;
    std::set<int> inventario;
    unsigned int energia = 100000;
    unsigned int consumo = 0;
    
    public:
    Jugador(std::string casa);
    ~Jugador();
    void aumentar_consumo(unsigned int consumo_);
    void reducir_consumo(unsigned int consumo_);
    std::string get_casa();
    bool hay_suficiente_energia(unsigned int costo);
    void agregar_elemento(int id,unsigned int costo,unsigned int energia_);
    void eliminar_elemento(int id,unsigned int energia_consumida);
    bool pertenece(int id);
};
}
#endif
