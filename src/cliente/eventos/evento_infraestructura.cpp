#include "cliente/eventos/evento_infraestructura.h"

#include "libs/json.hpp"

#include "conexion/eventos_cliente.h"
#include "cliente/modelo/juego.h"

namespace cliente {

EventoInfraestructura::EventoInfraestructura(const nlohmann::json& serializado) 
{
    data = serializado;
}

void EventoInfraestructura::actualizar(Juego& juego) {
    using namespace conexion;
    Infraestructura& infraestructura = juego.obtener_infraestructura();
    evento_cliente_t id_evento = (evento_cliente_t)data.at("id").get<int>();
    switch(id_evento) {
        case EVC_INICIAR_CONSTRUCCION:   // (id, clase, tiempo)
            infraestructura.iniciar_construccion(data.at("clase"), 
                data.at("tiempo"));
            break;
        case EVC_SINCRONIZAR_CONSTRUCCION: // (id, tiempo)
            infraestructura.sincronizar_construccion(data.at("clase"), 
                data.at("tiempo"));
            break;
        case EVC_ACTUALIZAR_COLA_CC:     // (clase, cantidad)
            infraestructura.actualizar_cola(data.at("clase"), 
                data.at("cantidad"));
            break;
        case EVC_SET_VELOCIDAD_CC:
            infraestructura.set_velocidad_construccion(data.at("velocidad"));
            break;
        case EVC_ATACAR_EDIFICIO:        // (id, nueva_vida)
            infraestructura.atacar(data.at("id_edificio"), 
                data.at("nueva_vida"));
            break;
        case EVC_CREAR_EDIFICIO:        // (id, posicion, id_jugador)
            infraestructura.crear_edificio(data.at("id_edificio"), 
                data.at("id_jugador"), data.at("clase"), data.at("posicion"));
            break;
        case EVC_AGREGAR_EDIFICIO:
            infraestructura.agregar_edificio(data.at("id_edificio"),
                data.at("id_jugador"), data.at("posicion"), data.at("clase"),
                data.at("vida"));
            break;
        case EVC_ELIMINAR_EDIFICIO:
            infraestructura.eliminar_edificio(data.at("id_edificio"));
            break;
        case EVC_DESTRUIR_EDIFICIO:      // (id)
            infraestructura.destruir_edificio(data.at("id_edificio"));
            break;
        default:
            // Tirar excepcion
            break;
    }
}

} // namespace cliente