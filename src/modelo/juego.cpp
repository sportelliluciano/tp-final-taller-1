#include <string>

#include "modelo/juego.h"
#include "modelo/infraestructura.h"
#include "modelo/ejercito.h"
#include "modelo/terreno.h"

namespace modelo {

void Juego::iniciar_partida(){
    //bla bla
    empezo = true;
}

    /**
     * \brief Devuelve true si la partida ya fue iniciada
     */
bool Juego::partida_iniciada() const {
    return empezo;
}

    /**
     * \brief Indica si el juego terminó
     */
bool Juego::juego_terminado() const {
    return !empezo;
}

    /**
     * \brief Actualiza un paso del modelo.
     * 
     * Actualiza un paso de `dt_ms` milisegundos en el modelo.
     * El parámetro `dt_ms` indica cuántos milisegundos pasaron entre 
     * la última vez que se llamó al método actualizar y este llamado.
     * Se garantiza que `dt_ms > 0`.
     */
void Juego::actualizar(int dt_ms){
    actualizar_construcciones(dt_ms);
    actualizar_tropas(dt_ms);
}

    /**
     * \brief Crea un nuevo jugador en el modelo con el ID y la casa indicada.
     * 
     * El nuevo jugador creado será agregado al juego en la casa indicada.
     * 
     * Si ya existe un jugador con dicho ID se lanzará una excepción.
     */
void Juego::crear_jugador(IJugador* jugador){
    jugadores.emplace(jugador->obtener_id(),Jugador("casa",jugador));
}

    /**
     * \brief Indica al modelo que el jugador se desconectó.
     */
void Juego::jugador_desconectado(IJugador* jugador){
}

    /**
     * \brief Iniciar la construcción de un edificio.
     * 
     * El jugador solicitó el inicio de la construcción de un edificio
     * de la clase indicada. El método devuelve true si la construcción
     * fue iniciada o false en caso contrario (falta de fondos, deshabilitado).
     */
bool Juego::iniciar_construccion_edificio(IJugador* jugador,
    const std::string& clase){
        unsigned int costo = inf.get_costo(clase);
        jugadores.at(jugador->obtener_id()).empezar_construccion(clase,costo);
        return true;
}
    
    /**
     * \brief Cancela la construcción de un edificio.
     * 
     * El jugador solicitó cancelar la construcción de un edificio
     * de la clase indicada. El método devuelve true si se cancelo la 
     * construccion o false en caso contrario (no había que cancelar).
     */
bool Juego::cancelar_construccion_edificio(IJugador* jugador, 
    const std::string& clase){
        unsigned int costo = inf.get_costo(clase);
        jugadores.at(jugador->obtener_id()).cancelar_construccion(clase,costo);
    return true;
}
    
    /**
     * \brief Ubica un edificio construido en el mapa.
     */
bool Juego::ubicar_edificio(IJugador* jugador, int celda_x, int celda_y,
        const std::string& clase){
    unsigned int consumo = inf.get_energia(clase);
    int id_edificio;
    if (jugadores.at(jugador->obtener_id()).hay_suficiente_energia(consumo)){
        id_edificio = inf.crear(clase,celda_x,celda_y);
        jugadores.at(jugador->obtener_id()).agregar_elemento(
                            id_edificio,consumo,clase);
        jugador->crear_edificio(
            id_edificio,
            clase, 
            celda_x, celda_y, 
            jugador->obtener_id()
        );
        return true;
    }
    return false;
}

    /**
     * \brief Vende un edificio del jugador.
     * 
     * El jugador solicitó vender el edificio indicado.
     * 
     * El método devuelve false si se produjo algún error lógico que impida la
     * venta del edificio.
     */
bool Juego::vender_edificio(IJugador* jugador, int id_edificio) {
    unsigned int consumo = inf.get_energia(id_edificio);
    for (auto it = jugadores.begin();it!= jugadores.end();++it){
        if ((it->second).pertenece(id_edificio)){
            (it->second).eliminar_elemento(id_edificio,consumo);
        }
    }
    unsigned int energia_retorno = inf.reciclar(id_edificio);//no se comunica al jugador
    jugador->eliminar_edificio(id_edificio);
}
    /**
     * \brief Inicia el entrenamiento de una tropa de la clase indicada.
     * 
     * Inicia el entrenamiento de una tropa indicada del jugador. 
     * 
     * El método devuelve false si no se puede iniciar el entrenamiento.
     */
bool Juego::iniciar_entrenamiento_tropa(IJugador* jugador,
    const std::string& clase){
        unsigned int costo = ejercito.get_costo(clase);
        jugadores.at(jugador->obtener_id()).empezar_entrenamiento(clase,costo);
        return true;
}

    /**
     * \brief Cancela el entrenamiento de una tropa de la clase indicada.
     * 
     * El método devuelve false si no se puede cancelar el entrenamiento (no 
     * hay nada que cancelar).
     */
bool Juego::cancelar_entrenamiento_tropa(IJugador* jugador,
    const std::string& clase){
        unsigned int costo = ejercito.get_costo(clase);
        jugadores.at(jugador->obtener_id()).cancelar_entrenamiento(clase,costo);
    return true;
}

    /**
     * \brief Inicia el movimiento de las tropas del jugador 
     *        a la posición (x_px, y_px), dada en píxeles globales.
     * 
     * El método devuelve false si no se puede mover a la posición indicada.
     */
bool Juego::mover_tropas(IJugador* jugador, const std::vector<int>& ids,
                                    int x, int y){
    unsigned int cant = sqrt(ids.size());
    unsigned int n = 0;
    for (int id : ids) {
        ejercito.mover(id_,x+n,y,jugador);
        n++;
        if (n == cant) {
            y ++;
            n = 0;
        }
    }
    return true;  
}

    /**
     * \brief El jugador quiere atacar con las tropas dadas por el ID a la
     *        tropa con id=id_atacado.
     * 
     * El método devuelve false si no se puede atacar (ej friendly-fire)
     */
bool Juego::atacar_tropa(IJugador* jugador, 
        const std::vector<int>& ids_atacantes, int id_atacado){
    std::string casa;
    for (auto it=jugadores.begin();it != jugadores.end();++it){
        if ((it->second).pertenece(id_atacado))casa = (it->second).get_casa();
    }        
    for (int id_ : ids_atacantes) {
        for (auto it=jugadores.begin();it != jugadores.end();++it){
            if ((it->second).pertenece(id_) && (it->second).get_casa()==casa)return false;
        }
        ejercito.atacar(id_atacado,id_);
    }
    return true;
}

    /**
     * \brief El jugador quiere indicarle a la(s) cosechadora(s) que vayan
     *        a recolectar especia a la celda indicada.
     */
bool Juego::indicar_especia_cosechadora(IJugador* jugador,
    const std::vector<int>& ids, int celda_x, int celda_y) {}


Juego::Juego():terreno(Terreno("../data/terreno.csv")),
                        inf(Infraestructura(terreno)),
                        ejercito(Ejercito(terreno))
{}
Juego::~Juego(){
}
/*
void Juego::destruir_unidad(int id_jugador,int id_){
    ejercito.destruir(id_);
    jugadores.at(id_jugador).eliminar_elemento(id_,0);
}
void Juego::atacar_unidad(int id_jugador,int id_victima,int id_atacante){
    ejercito.atacar(id_victima,id_atacante);
}
void Juego::atacar_edificio(int id_jugador,int id_edificio,int id_atacante){
    ejercito.atacar(inf.get(id_edificio),id_atacante);
}*/
Unidad& Juego::get_unidad(int id_){//temporal
    return ejercito.get(id_);
}

//------------------------------------------------------------------------------------
void Juego::actualizar_construcciones(int dt) {
    for (auto it=jugadores.begin();it != jugadores.end();++it){
        (it->second).actualizar_construcciones(dt,inf);
    }
}
void Juego::actualizar_tropas(int dt) {//se necesita un IJugador!!!
    Posicion& pos = inf.get_posicion("Centro de Construcción");
    for (auto it=jugadores.begin();it != jugadores.end();++it){
        std::string actualizacion=(it->second).actualizar_tropas(dt,ejercito);
        if ( actualizacion != "ok"){// se termino el entrenamiento 
            ejercito.crear(actualizacion,pos);
        }
    }
    ejercito.actualizar_tropas(dt,comunicacion_jugador);//el puntero no apunta a nada
}
}
