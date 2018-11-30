#include "cliente/modelo/infraestructura.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "libs/json.hpp"

#include "cliente/modelo/terreno.h"
#include "cliente/sonido/sonido.h"

/**
 * Los contadores de tiempo de construcción se reducen hasta un mínimo.
 * El 0 en los contadores lo setea el servidor.
 */
#define MIN_TIEMPO_CONSTRUCCION 1

namespace cliente {

Infraestructura::Infraestructura(int id_jugador_actual_, Terreno& terreno_juego,
    const nlohmann::json& data_edificios) 
: terreno(terreno_juego),
  id_jugador_actual(id_jugador_actual_)
{ 
    auto it = data_edificios.begin();
    const nlohmann::json& valores_por_defecto = *it;
    ++it;
    for(; it != data_edificios.end(); ++it) {
        // Mergear valores por defecto con el elemento actual
        nlohmann::json elem = valores_por_defecto;
        elem.update(*it);

        edificios.emplace(elem.at("id"), Edificio(elem));
        if (elem.at("id") != "centro_construccion")
            edificios_base_ordenados.push_back(&edificios.at(elem.at("id")));
    }
}

void Infraestructura::renderizar(Ventana& ventana, Camara& camara) {
    for (Edificio* edificio : 
        terreno.obtener_edificios_en(camara.obtener_vista())) 
    {
        Posicion visual;
        int x_celda = edificio->obtener_celda_x(),
            y_celda = edificio->obtener_celda_y();

#ifdef DEPURACION_DIBUJO
        /*** Pintar celda ***/
        for (int x=0; x<edificio->obtener_ancho_celdas();x++) {
            for (int y=0; y<edificio->obtener_alto_celdas();y++) {
                visual = camara.traducir_a_visual(
                    terreno.obtener_posicion(x + x_celda, y + y_celda));
                Sprite(0).renderizar(ventana, visual.x, visual.y);
            }
        }
        /*** Fin pintar celda ***/
#endif

        Posicion esq_sup = camara.traducir_a_visual(
            terreno.obtener_posicion(x_celda, y_celda));
        Posicion esq_inf = camara.traducir_a_visual(
            terreno.obtener_posicion(
                x_celda + edificio->obtener_ancho_celdas(),
                y_celda + edificio->obtener_alto_celdas()
            ));
        
        int offset_x = (esq_inf.x - esq_sup.x) / 2;
        int offset_y = (esq_inf.y - esq_sup.y) / 2;
        

        visual = camara.traducir_a_visual(terreno.obtener_posicion(edificio));
        edificio->renderizar(ventana, visual.x + offset_x, visual.y + offset_y);
    }
}

void Infraestructura::actualizar(int t_ms) {
    if (last_ms == -1)
        last_ms = t_ms;
    
    int dt_cc = (int)((t_ms - last_ms) * velocidad_cc);
    
    for (auto it = construcciones_iniciadas.begin(); 
        it != construcciones_iniciadas.end(); ++it) 
    {
        if (it->second <= MIN_TIEMPO_CONSTRUCCION)
            continue;
        if (it->second - dt_cc < MIN_TIEMPO_CONSTRUCCION)
            it->second = MIN_TIEMPO_CONSTRUCCION;
        else
            it->second -= dt_cc;
    }

    std::vector<int> edificios_a_eliminar;

    for (auto& par : edificios_construidos) {
        Edificio& edificio = par.second;
        if (!edificio.esta_vivo()) {
            edificios_a_eliminar.push_back(edificio.obtener_id());
        } else {
            edificio.actualizar(t_ms - last_ms);
        }
    }

    for (int id_a_eliminar: edificios_a_eliminar) {
        edificios_construidos.erase(id_a_eliminar);
    }
    
    last_ms = t_ms;
}

bool Infraestructura::existe(int id_edificio) const {
    return edificios_construidos.count(id_edificio) != 0;
}

Edificio& Infraestructura::obtener(int id_edificio) {
    return edificios_construidos.at(id_edificio);
}

bool Infraestructura::hay_edificio_enemigo_en(const Posicion& punto) const {
    Edificio* edificio = terreno.obtener_edificio_en(punto);
    if (edificio) {
        return edificio->obtener_propietario() != id_jugador_actual;
    }
    return false;
}

Edificio& Infraestructura::obtener_edificio_enemigo_en(const Posicion& punto) {
    Edificio* edificio = terreno.obtener_edificio_en(punto);
    if (edificio && edificio->obtener_propietario() != id_jugador_actual)
        return *edificio;
    
    throw std::runtime_error("No hay un edificio enemigo en el punto");
}

Edificio* Infraestructura::obtener_centro_construccion() {
    for (auto& it : edificios_construidos) {
        if (it.second.obtener_clase() == "centro_construccion") {
            if (it.second.obtener_propietario() == id_jugador_actual)
                return &it.second;
        }
    }
    return nullptr;
}

const Edificio& Infraestructura::obtener_edificio_base(
        const std::string& clase) const 
{
    return edificios.at(clase);
}

const std::vector<const Edificio*>& 
    Infraestructura::obtener_edificios_base() const 
{
    return edificios_base_ordenados;
}

bool Infraestructura::esta_construyendo(const std::string& clase) const {
    return construcciones_iniciadas.find(clase) != 
        construcciones_iniciadas.end();
}

int Infraestructura::obtener_cola_construccion(const std::string& clase) const {
    auto val = colas_construccion.find(clase);
    if (val == colas_construccion.end())
        return 0;
    return val->second;
}

int Infraestructura::obtener_segundos_restantes(const std::string& clase) const 
{
    return construcciones_iniciadas.at(clase);
}

bool Infraestructura::jugador_actual_tiene(const std::string& clase) const {
    for (auto& it : edificios_construidos) {
        if (it.second.obtener_clase() == clase) {
            if (it.second.obtener_propietario() == id_jugador_actual)
                return true;
        }
    }
    return false;
}

void Infraestructura::iniciar_construccion(const std::string& clase, 
    int tiempo_ms)
{
    construcciones_iniciadas[clase] = tiempo_ms;
    Sonido::reproducir_sonido(SND_CONSTRUYENDO);
}

void Infraestructura::sincronizar_construccion(const std::string& clase, 
    int tiempo_ms) 
{
    if (tiempo_ms == 0) {
        Sonido::reproducir_sonido(SND_CONSTRUCCION_TERMINADA);
    }
    construcciones_iniciadas[clase] = tiempo_ms;
}

void Infraestructura::set_velocidad_construccion(float velocidad) {
    velocidad_cc = velocidad;
}

void Infraestructura::actualizar_cola(const std::string& clase, int cantidad) {
    colas_construccion[clase] = cantidad;
}

void Infraestructura::atacar(int id, int nueva_vida) {
    Edificio& atacado = edificios_construidos.at(id);
    if ((atacado.obtener_clase() == "centro_construccion") && 
        (atacado.obtener_propietario() == id_jugador_actual))
    {
        Sonido::reproducir_sonido(SND_BASE_BAJO_ATAQUE);
    }

    edificios_construidos.at(id).set_vida(nueva_vida);
}

void Infraestructura::crear_edificio(int id, int id_jugador, 
    const std::string& clase, const std::vector<int>& posicion)
{           
    if (id_jugador == id_jugador_actual) {
        construcciones_iniciadas.erase(clase);
    }
    Edificio nuevo = edificios.at(clase);

    // El servidor tiene celdas de 8x8
    int x = (posicion.at(0) * 8) / 32, y = (posicion.at(1) * 8) / 32;


    nuevo.inicializar(id, x, y, id_jugador, false);
    edificios_construidos.emplace(id, nuevo);
    terreno.agregar_edificio(edificios_construidos.at(id));
}

void Infraestructura::agregar_edificio(int id, int id_jugador, 
    const std::vector<int>& posicion, const std::string& clase, int vida)
{
    Edificio nuevo = edificios.at(clase);

    int x = (posicion.at(0) * 8) / 32, y = (posicion.at(1) * 8) / 32;

    nuevo.inicializar(id, x, y, id_jugador, true, vida);
    edificios_construidos.emplace(id, nuevo);
    terreno.agregar_edificio(edificios_construidos.at(id));
}

void Infraestructura::eliminar_edificio(int id) {
    terreno.eliminar_edificio(edificios_construidos.at(id));
    edificios_construidos.erase(id);
}

void Infraestructura::destruir_edificio(int id) {
    Edificio& destruido = edificios_construidos.at(id);
    if (destruido.obtener_propietario() == id_jugador_actual)
        Sonido::reproducir_sonido(SND_EDIFICIO_PERDIDO);
    terreno.eliminar_edificio(destruido);
    destruido.destruir();
}

int Infraestructura::obtener_sprite_clase(const std::string& clase) const {
    return edificios.at(clase).obtener_sprite_boton();
}

} // namespace cliente
