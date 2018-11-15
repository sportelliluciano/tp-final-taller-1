#ifndef _JUGADOR_H_
#define _JUGADOR_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cliente/modelo/ejercito.h"
#include "cliente/modelo/infraestructura.h"
#include "cliente/modelo/terreno.h"
#include "cliente/video/camara.h"
#include "cliente/video/ventana.h"

namespace cliente {

class Jugador {
public:
    Jugador(Terreno& terreno_juego);
    
    /**
     * \brief Renderiza las unidades y edificios del jugador en la ventana.
     */
    void renderizar(Ventana& ventana, Camara& camara);

    /**
     * \brief Actualiza el estado del jugador.
     */
    void actualizar(int t_ms);
    
    /**
     * \brief Obtiene el dinero del jugador.
     */
    int obtener_dinero() const;

    /**
     * \brief Setea el dinero del jugador.
     */
    void setear_dinero(int dinero_);
    
    std::vector<const Edificio*> obtener_edificios() const;
    
    /**
     * \brief Obtiene la infraestructura del jugador.
     */
    Infraestructura& obtener_infraestructura();

    /**
     * \brief Obtiene el ejército del jugador.
     */
    Ejercito& obtener_ejercito();

private:
    int energia = 0;
    int dinero = 0;
    Infraestructura infraestructura;
    Ejercito tropas;
};

} // namespace cliente

#endif // _JUGADOR_H_
