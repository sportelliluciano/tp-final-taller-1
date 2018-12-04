#include "modelo/juego.h"

#include <string>
#include <iostream>
#include <vector>
#include <set>

#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

#define DINERO_INICIAL         10000
#define DINERO_MAXIMO_INICIAL  10000
#define ENERGIA_INICIAL        1000
#define ENERGIA_MAXIMA_INICIAL 1000

namespace modelo {

Juego::Juego() 
: inf(comunicacion_jugadores,id), ejercito(comunicacion_jugadores,id) 
{ 
}

void Juego::inicializar(const nlohmann::json& mapa, 
    const nlohmann::json& edificios, const nlohmann::json& ejercito_)
{
    json_edificios = &edificios;
    json_ejercitos = &ejercito_;
    json_mapa = &mapa;
    for (const nlohmann::json& data : mapa.at("jugadores")) {
        posiciones_centros.push_back(data);
    }

    terreno.inicializar(mapa);
    inf.inicializar(&terreno,edificios);
    ejercito.inicializar(&terreno,ejercito_);
}

void Juego::crear_jugador(IJugador* jugador) {
    jugadores.emplace(
        jugador->obtener_id(),
        Jugador(jugador->obtener_casa(), jugador));
    comunicacion_jugadores.agregar_jugador(jugador);
}

void Juego::iniciar_partida() {
    empezo = true;
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->inicializar(j->obtener_id(), *json_mapa, *json_edificios, 
            *json_ejercitos);
        j->actualizar_dinero(DINERO_INICIAL, DINERO_MAXIMO_INICIAL);
        j->actualizar_energia(ENERGIA_INICIAL, ENERGIA_MAXIMA_INICIAL);
    });
    
    int i = 0;
    for (auto it = jugadores.begin(); it != jugadores.end(); ++it) {
        int id_jugador = it->first;
        Jugador& jugador = it->second;
        int x = posiciones_centros[i][0],
            y = posiciones_centros[i][1];
        int nuevo_id = inf.crear_centro_construccion(x, y, id_jugador);
        terreno.agregar_centro(x,y,id_jugador);
        // TODO: deshardcodear ese centro_construcciones
        jugador.agregar_elemento(nuevo_id, 0, "centro_construcciones");
        i++;

        comunicacion_jugadores.broadcast([&] (IJugador* j) {
            IJugador *otro = jugador.get_jugador();
            if (j != otro) {
                j->crear_jugador(otro->obtener_id(), otro->obtener_nombre(),
                    otro->obtener_casa());
            }
        });
    }
}

bool Juego::esperar_sincronizacion_inicial() {
    for (auto it = jugadores.begin(); it != jugadores.end(); ++it) {
        Jugador& jugador = it->second;
        
        if (!jugador.inicio_sincronizado())
            return false;
    }

    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        j->juego_iniciando();
    });
    return true;
}

void Juego::actualizar(int dt_ms) {
    // Procesar eventos que no hay que broadcastear
    for (auto it=jugadores.begin(); it != jugadores.end(); ++it) {
        Jugador& jugador = it->second;
        jugador.actualizar_construcciones(dt_ms, inf);
        jugador.actualizar_entrenamientos(dt_ms, tropas_a_construir,
                                        ejercito.get_tiempos_entrenamiento());
        for (auto it2=tropas_a_construir.begin();
                 it2!=tropas_a_construir.end(); ++it2){
            int nuevo_id;
            if (*it2 == "cosechadora"){
                nuevo_id = ejercito.crear_cosechadora(*it2,
                jugador.get_jugador()->obtener_id(),&jugador);
            } else{
                nuevo_id = ejercito.crear(*it2, 
                    jugador.get_jugador()->obtener_id());
            } 
            jugador.agregar_elemento(nuevo_id, 0, *it2);
        }
    }

    // Procesar eventos que sí hay que broadcastear
    inf.actualizar_edificios(dt_ms);
    ejercito.actualizar_tropas(dt_ms);
    // actualizo las bajas en el modelo
    std::unordered_set<int>& bajas = ejercito.notificar_bajas(); 
    for (auto it = bajas.begin(); it != bajas.end(); ++it) {
        for (auto it2=jugadores.begin(); it2!=jugadores.end(); ++it2){
            Jugador& jugador = it2->second;
            if (jugador.pertenece(*it)){
                if (inf.pertenece(*it)){
                    unsigned int consumo = inf.get_energia(*it);
                    inf.destruir(*it, (jugador.get_jugador())->obtener_id());
                    jugador.eliminar_elemento(*it,consumo);

                } else {
                    jugador.eliminar_elemento(*it,0);
                }
            }    
        }
    }

    std::vector<int> centros = inf.get_centros_construcciones();
    if (centros.size() == 1) {
        for (auto it=jugadores.begin(); it!=jugadores.end(); ++it){
            Jugador& jugador = it->second;
            if (jugador.pertenece(centros.at(0))) {
                comunicacion_jugadores.broadcast([&] (IJugador *j) {
                    j->juego_terminado(jugador.get_jugador()->obtener_nombre());
                });
                empezo = false;
                return;
            }    
        }
    }
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

void Juego::jugador_listo(IJugador* jugador) {
    jugadores.at(jugador->obtener_id()).inicio_sincronizado(true);
    comunicacion_jugadores.broadcast([&] (IJugador* j) {
        if (j != jugador) {
            j->jugador_listo(jugador->obtener_id());
        }
    });
}

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
    Jugador& jugador = jugadores.at(conexion_jugador->obtener_id());

    if (!jugador.ubicar_edificio(clase, celda_x, celda_y, inf)) {
        std::cout << "---------------Dentro del modelo---------------" <<'\n';
        std::cout << "No se ubico el edificio" << std::endl;
    } else {
        if (clase=="refineria"){
            std::cout << "Se creo una refineria" << std::endl;
            terreno.agregar_refineria(celda_x,celda_y,
                    conexion_jugador->obtener_id());
        }
    }
}

void Juego::vender_edificio(IJugador* jugador, int id_edificio) {
    std::cout << "voy a vender: "<<inf.get(id_edificio).get_tipo()<< std::endl;
    if (inf.get(id_edificio).get_tipo() == "centro_construccion"){
        std::cout << "no se puede vender" << std::endl;
        return;
    }
    if (!jugadores.at(jugador->obtener_id()).pertenece(id_edificio)){
        std::cout << "no es tuyo gil" << std::endl;
        return;
    }

    unsigned int consumo = inf.get_energia(id_edificio);
    for (auto it = jugadores.begin(); it!= jugadores.end(); ++it){
        if ((it->second).pertenece(id_edificio)){
            (it->second).eliminar_elemento(id_edificio,consumo);
            unsigned int plata_retorno = inf.reciclar(id_edificio,
                jugador->obtener_id());
            (it->second).aumentar_plata(plata_retorno);
            return;
        }
    }
}

void Juego::iniciar_entrenamiento_tropa(IJugador* jugador,
    const std::string& clase)
{
    std::set<std::string>& requisitos = ejercito.get_requisitos(clase);
    Jugador& jugador_ = jugadores.at(jugador->obtener_id());
    if (!jugador_.tiene(requisitos,inf)||!ejercito.pertenece(clase,
            jugador->obtener_casa()))
        return;
    
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
        ejercito.mover(id_, celda_x + n, celda_y);
        n++;
        if (n == cant) {
            celda_y++;
            n = 0;
        }
    }
}

void Juego::atacar(IJugador* jugador, 
        const std::unordered_set<int>& ids_atacantes, int id_atacado)
{
    Jugador& atacante = jugadores.at(jugador->obtener_id());
    for (auto it = ids_atacantes.begin(); it != ids_atacantes.end(); ++it){
        if (atacante.pertenece(id_atacado) && atacante.pertenece(*it)){
            continue;
        }
        if (inf.pertenece(id_atacado)) {
            ejercito.atacar(&(inf.get(id_atacado)),*it);
        } else{  
            ejercito.atacar(id_atacado,*it);
        }
    }
}

void Juego::indicar_especia_cosechadora(IJugador* jugador,
    const std::unordered_set<int>& ids, int celda_x, int celda_y) {
}

} // namespace modelo
