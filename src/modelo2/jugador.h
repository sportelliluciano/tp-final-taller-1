#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "comun/i_modelo.h"
#include "comun/i_jugador.h"
#include "modelo/infraestructura.h"
//#include "modelo/ejercito.h"

namespace modelo {

class Jugador{
    private:
    bool sincronizado = false;
    std::string casa;
    std::set<int> inventario;
    int energia = 100000;
    int plata = 10000;
    unsigned int energia_max = 10000000;
    IJugador* comunicacion_jugador;
    std::unordered_map<std::string, int> construcciones;
    std::unordered_set<std::string> construcciones_esperando_ubicacion;
    std::unordered_map<std::string, int> construcciones_en_cola;

    std::unordered_map<std::string, int> tropas;
    std::unordered_map<std::string, int> tropas_en_cola;

    public:
    Jugador(std::string casa,IJugador* jugador);
    ~Jugador();
    void aumentar_energia(unsigned int energia_);
    void reducir_energia(unsigned int energia_);
    void aumentar_plata(unsigned int ganancia);
    void reducir_plata(unsigned int costo);
    std::string get_casa();
    bool hay_suficiente_energia(unsigned int costo);
    bool empezar_construccion(const std::string& clase,unsigned int costo);
    bool empezar_entrenamiento(const std::string& clase,unsigned int costo);
    bool cancelar_construccion(const std::string& clase,unsigned int costo);
    bool cancelar_entrenamiento(const std::string& clase,unsigned int costo);
    bool ubicar_edificio(const std::string& clase, int celda_x, int celda_y, Infraestructura& inf);
    void agregar_elemento(int id,unsigned int energia_,const std::string& clase);
    void eliminar_elemento(int id,unsigned int energia_consumida);
    bool pertenece(int id);
    void actualizar_construcciones(int dt, Infraestructura& infraestructura);
    void actualizar_entrenamientos(int dt,std::vector<std::string>& nuevas_tropas,
                                    std::unordered_map<std::string,int>& tiempos_de_entrenamiento);
    IJugador* get_jugador();
    bool tiene(std::set<std::string>& requisitos,Infraestructura& inf);
    float obtener_varaible_de_entrenamiento(std::set<std::string>& requisitos,Infraestructura& inf);

    /**
     * \brief Devuelve true si el jugador ya sincronizó su inicio de juego.
     */
    bool inicio_sincronizado() const;

    /**
     * \brief Marca o desmarca al jugador indicando que ya sincronizó su inicio
     *        de juego.
     */
    void inicio_sincronizado(bool activar);
};
}
#endif
