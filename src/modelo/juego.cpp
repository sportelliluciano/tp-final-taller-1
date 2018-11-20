#include <string>
#include <iostream>

#include "modelo/juego.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

Juego::Juego() 
: inf(comunicacion_jugadores), ejercito(comunicacion_jugadores) 
{ }

void Juego::inicializar(const nlohmann::json& mapa, 
        const nlohmann::json& edificios, const nlohmann::json& ejercito_)
{
    terreno.inicializar(mapa);
    inf.inicializar(&terreno,edificios);
    ejercito.inicializar(&terreno,ejercito_);
}

void Juego::crear_jugador(IJugador* jugador) {
    jugadores.emplace(
        jugador->obtener_id(),
        Jugador(jugador->obtener_casa(), jugador)
    );
    comunicacion_jugadores.agregar_jugador(jugador);
}

void Juego::iniciar_partida() {
    //bla bla
    empezo = true;
}

void Juego::actualizar(int dt_ms) {
    // Procesar eventos que no hay que broadcastear
    for (auto it=jugadores.begin(); it != jugadores.end(); ++it) {
        Jugador& jugador = it->second;
        jugador.actualizar_construcciones(dt_ms, inf);
        jugador.actualizar_entrenamientos(dt_ms, ejercito);
    }

    // Procesar eventos que sí hay que broadcastear
    inf.actualizar_edificios(dt_ms);
    ejercito.actualizar_tropas(dt_ms);
}

bool Juego::partida_terminada() const {
    return !empezo;
}

void Juego::jugador_desconectado(IJugador* jugador) {
    // NOTA: Agrego este código por un tema de estabilidad.
    // Luego de ejecutado este método, jugador ya no es válido.
    jugadores.erase(jugador->obtener_id());
    comunicacion_jugadores.eliminar_jugador(jugador);
}

Juego::~Juego() { }

/**** Mensajes desde los jugadores ****/

void Juego::iniciar_construccion_edificio(IJugador* jugador,
    const std::string& clase)
{
    unsigned int costo = inf.get_costo(clase);
    jugadores.at(jugador->obtener_id()).empezar_construccion(clase,costo);
}

void Juego::cancelar_construccion_edificio(IJugador* jugador, 
    const std::string& clase)
{
    unsigned int costo = inf.get_costo(clase);
    jugadores.at(jugador->obtener_id()).cancelar_construccion(clase,costo);
}
    
void Juego::ubicar_edificio(IJugador* conexion_jugador, int celda_x, 
    int celda_y, const std::string& clase)
{
    unsigned int consumo = inf.get_energia(clase);
    int id_edificio;
    Jugador& jugador = jugadores.at(conexion_jugador->obtener_id());

    if (!jugador.ubicar_edificio(clase, celda_x, celda_y, inf)) {
        std::cout << "------------------Dentro del modelo------------------" <<'\n';
        std::cout << "No se ubico el edificio" << std::endl;
    } else {
        if (clase=="refineria")
            terreno.agregar_refineria(celda_x,celda_y,conexion_jugador->obtener_id());
    }
}

void Juego::vender_edificio(IJugador* jugador, int id_edificio) {
    unsigned int consumo = inf.get_energia(id_edificio);
    bool esta_bien = false;
    for (auto it = jugadores.begin();it!= jugadores.end();++it){
        if ((it->second).pertenece(id_edificio)){
            (it->second).eliminar_elemento(id_edificio,consumo);
            unsigned int energia_retorno = inf.reciclar(id_edificio);
            (it->second).aumentar_energia(energia_retorno);
            esta_bien = true;
        }
        ((it->second).get_jugador())->eliminar_edificio(id_edificio);
    }
}

void Juego::iniciar_entrenamiento_tropa(IJugador* jugador,
    const std::string& clase)
{
    unsigned int costo = ejercito.get_costo(clase);
    jugadores.at(jugador->obtener_id()).empezar_entrenamiento(clase,costo);
}

void Juego::cancelar_entrenamiento_tropa(IJugador* jugador,
    const std::string& clase)
{
    unsigned int costo = ejercito.get_costo(clase);
    jugadores.at(jugador->obtener_id()).cancelar_entrenamiento(clase,costo);
}

void Juego::mover_tropas(IJugador* jugador, const std::unordered_set<int>& ids,
    int x, int y)
{
    unsigned int cant = sqrt(ids.size());
    unsigned int n = 0;
    unsigned int celda_x = x / 8;
    unsigned int celda_y = y / 8;
    for (int id_ : ids) {
        ejercito.mover(id_, celda_x + n, celda_y, jugador);
        n++;
        if (n == cant) {
            celda_y++;
            n = 0;
        }
    }
}

void Juego::atacar_tropa(IJugador* jugador, 
        const std::unordered_set<int>& ids_atacantes, int id_atacado)
{
    std::string casa;
    for (auto it=jugadores.begin();it != jugadores.end();++it){
        if ((it->second).pertenece(id_atacado)){
            casa = (it->second).get_casa();
            break;
        }
    }        
    for (int id_ : ids_atacantes) {
        for (auto it=jugadores.begin();it != jugadores.end();++it){
            //if ((it->second).pertenece(id_) && (it->second).get_casa()==casa)return;
        }
        ejercito.atacar(id_atacado,id_);
    }
}

void Juego::indicar_especia_cosechadora(IJugador* jugador,
    const std::unordered_set<int>& ids, int celda_x, int celda_y) 
{

}

}
