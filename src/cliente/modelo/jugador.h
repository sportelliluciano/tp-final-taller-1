#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Jugador {
public:
    Jugador(Terreno& terreno_juego);
    void renderizar(Ventana& ventana);
    void actualizar(int t_ms);
    void iniciar_construccion(const std::string& clase);
    void construir_edificio(int id, const std::string& clase, int x, int y);
    void destruir_edificio(int id);
    void crear_tropa(int id, const std::string& clase, int x, int y);
    void destruir_tropa(int id);
    int obtener_dinero() const;
    void setear_dinero(int dinero_);
    std::vector<const Edificio*> obtener_edificios() const;
    void seleccionar_unidades(const std::unordered_set<const Tropa*>& unidades);
    void seleccionar_edificio(const Edificio& edificio);
    void deseleccionar_edificio();
    void sincronizar_tropa(int id_tropa, int x, int y);
    void indicar_camino_tropa(int id_tropa, 
        const std::vector<std::pair<int, int>>& camino);
private:
    Terreno& terreno;
    int energia = 0;
    int dinero = 0;
    Infraestructura infraestructura;
    Ejercito tropas;
};

} // namespace cliente

#endif // _JUGADOR_H_
