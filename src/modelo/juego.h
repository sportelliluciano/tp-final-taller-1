#ifndef _JUEGO_H_
#define _JUEGO_H_

#include <string>
#include <unordered_map>

#include "modelo/id.h"
#include "modelo/jugador.h"
#include "modelo/unidad.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"
#include "modelo/i_jugador.h"
namespace modelo {

class Juego{
    private:
    Infraestructura inf;
    Ejercito ejercito;
    Terreno& terreno;
    std::unordered_map<int,Jugador> jugadores;
    Id id;
    bool empezo = false;
    void actualizar_construcciones(int dt) ;
    void actualizar_tropas(int dt);
    IJugador* comunicacion_jugador;

    public:
    Juego(Terreno& terreno);
    ~Juego();
    void iniciar_partida();
    bool partida_iniciada();
    bool juego_terminado();
    void crear_jugador(IJugador* jugador);
    void jugador_desconectado(IJugador* jugador);//no implementada
    bool iniciar_construccion_edificio(IJugador* jugador,
        const std::string& clase);
    bool cancelar_construccion_edificio(IJugador* jugador, 
        const std::string& clase);
    bool ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase);    
    bool vender_edificio(IJugador* jugador, int id_edificio);//sin implementar
    bool iniciar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);
    bool cancelar_entrenamiento_tropa(IJugador* jugador,
        const std::string& clase);
    bool mover_tropas(IJugador* jugador, const std::vector<int>& ids,
                                    int x, int y);
    bool atacar_tropa(IJugador* jugador, 
        const std::vector<int>& ids_atacantes, int id_atacado);//le falta                                    
    bool indicar_especia_cosechadora(IJugador* jugador,
    const std::vector<int>& ids, int celda_x, int celda_y);//sin implementar

    void actualizar(int dt_ms);
    void eliminar_jugador(int id);
    void crear_edificio(int id_jugador,std::string id_tipo,int x,int y);
    void reciclar_edificio(int id_jugador,int id);
    void destruir_edificio(int id_jugador,int id);
    Edificio& get(int id);//temporal
    void crear_unidad(int id_jugador,std::string id_tipo,int x,int y);
    void destruir_unidad(int id_jugador,int id);
    void mover_unidad(int id_jugador,int id,int x,int y);
    void atacar_unidad(int id_jugador,int id_victima,int id_atacante);
    void atacar_edificio(int id_jugador,int id_edificio,int id_atacante);
    Unidad& get_unidad(int id);
};
}
#endif
