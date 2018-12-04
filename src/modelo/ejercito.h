#ifndef _EJERCITO_H_
#define _EJERCITO_H_

#include <unordered_map>
#include <string>
#include <unordered_set>
#include <set>

#include "modelo/terreno.h"
#include "modelo/unidad.h"
#include "modelo/ejercito_creador.h"
#include "modelo/arma_creador.h"
#include "modelo/edificio.h"
#include "modelo/id.h"
#include "comun/i_jugador.h"
#include "modelo/cosechadora.h"
#include "modelo/broadcaster.h"
#include "modelo/gusano.h"
#include "modelo/jugador.h"

namespace modelo {
/**
 * \brief Ejercito. 
 * Se encarga de gestionar y almacenar todas las unidades dentro del juego.
 * Tropas: almacena todas las unidades del juego, con el formato 
 * id:Unidad.
 * Cosechadoras: almacena todas las cosechadoras del juego, con el formato 
 * id:Cosechadora.
 * Prototipos: instancia de EjercitoCreador(encargada de crear las unidades
 * y cosechadoras).
 * Terreno: puntero al terreno(unica instancia) del juego.
 * Gusano: unica instancia del gusano del juego.
 */
class Ejercito{
    private:
    Broadcaster& comunicacion_jugadores;
    std::unordered_map<int,Unidad> tropas;
    std::unordered_map<int,Cosechadora> cosechadoras;
    EjercitoCreador prototipos;
    Terreno* terreno;
    Id& id_;
    std::unordered_set<int> tropas_en_movimiento;
    std::unordered_set<int> tropas_atacando;
    std::unordered_set<int> tropas_muertas;
    Gusano gusano;
    std::unordered_map<std::string,int> tiempos_de_entrenamiento;

    Atacable& obtener_unidad_aleatorio();

    public:
    Ejercito(Broadcaster& broadcaster,Id& id);
    /**
     * \brief Inicializador.
     *  
     * Inicializa todos los prototipos a partir de ejercito, parametro en formato JSON.
     */
    void inicializar(Terreno* terreno,const nlohmann::json& ejercito_);
    ~Ejercito();
    /**
     * \brief crea una unidad.
     *  
     * crea una instancia de unidad de tipo igaul a id_tipo.
     */
    int crear(const std::string& id_tipo, int id_propietario);
    /**
     * \brief crea una cosehcadora.
     *  
     * crea una instancia de unidad de tipo igaul a id_tipo.
     */
    int crear_cosechadora(const std::string& id_tipo,int id_propietario,
        Jugador* comunicacion_jugador);
    /**
     * \brief Inicializa el movimiento de una unidad hacia la posicion (x,y).
     */ 
    void mover(int id,int x,int y);
    /**
     * \brief Inicializa el movimiento de una cosechadora hacia la posicion (x,y).
     */ 
    void mover_cosechadora(int id,int x,int y);
    /**
     * \brief Inicializa el ataque de la tropa con id igual a id_atacante hacia
     * la tropa con id igual a id_victima.
     */ 
    void atacar(int id_victima,int id_atacante);
    /**
     * \brief Inicializa el ataque de la tropa con id igual a id_atacante hacia
     * el edificio.
     */ 
    void atacar(Atacable* edificio,int id_atacante);
    /**
     * \brief Configura al edificio para que sea eliminado al actualizarse el modelo.
     */
    void matar_edificio(int id_edficio_victima);
    /**
     * \brief Configura a la unidad (o cosechadora) para que sea 
     *  eliminado al actualizarse el modelo.
     */
    void matar_tropa(int id_victima,int id_atacante);

    /**
     * \brief Devuelve un set con las bajas de la ultima actualizacion.
     */
    std::unordered_set<int>& notificar_bajas();
    /**
     * \brief Actualizacion temporal para el gusano.
     */
    void actualizar_gusano(int dt);
    /**
     * \brief Actualizacion temporal para las cosechadoras.
     */
    void actualizar_cosechadoras(int dt,Cosechadora& cosechadora);
    /**
     * \brief Actualizacion temporal para el movimiento de tropas.
     */
    void actualizar_movimiento(int dt);
    /**
     * \brief verifica que la set de tropas atacando quede actualizado y 
     * contanga solo unidades con estado correcto.
     */
    void limpiar_tropas_atacando();
    /**
     * \brief Bora del modelo a las tropas muertas.
     */
    void eliminar_tropas();
    /**
     * \brief Actualizacion temporal para el el ataque de las undiades.
     */
    void actualizar_ataques(int dt);
    /**
     * \brief Actualizacion temporal para el ataque y movimiento de las unidades.
     */
    void actualizar_tropas(int dt);

    std::unordered_map<std::string,int>& get_tiempos_entrenamiento();
    Unidad& get(int id);
    unsigned int get_costo(const std::string& id_tipo);
    unsigned int get_tiempo(const std::string& id_tipo);
    std::set<std::string>& get_requisitos(const std::string& clase);
    bool pertenece(const std::string& clase,const std::string& casa);
};

} // namespace modelo

#endif // _EJERCITO_H_
