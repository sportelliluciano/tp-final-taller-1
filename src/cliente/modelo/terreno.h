#ifndef _TERRENO_H_
#define _TERRENO_H_

#include <functional>
#include <vector>
#include <unordered_set>

#include "cliente/modelo/celda.h"
#include "cliente/video/camara.h"
#include "cliente/video/posicion.h"
#include "cliente/video/ventana.h"

namespace cliente {

/**
 * \brief Terreno de juego.
 * 
 * Representa una grilla que contiene el terreno de juego.
 */
class Terreno {
public:
    /**
     * \brief Constructor. Carga el terreno a partir de un archivo CSV.
     */
    Terreno(const char *ruta_csv);
    
    /**
     * \brief Renderiza el terreno en la ventana indicada.
     */
    void renderizar(Ventana& ventana, Camara& camara);

    /**
     * \brief Obtiene la posición en píxeles globales de la celda indicada.
     */
    Posicion obtener_posicion(int celda_x, int celda_y);

    /**
     * \brief Obtiene la posición en píxeles globales del edificio indicado.
     */
    Posicion obtener_posicion(const Edificio* edificio);

    /**
     * \brief Obtiene la posición en píxeles globales de la tropa indicada.
     */
    Posicion obtener_posicion(const Tropa* tropa);

    /**
     * \brief Devuelve los edificios que se encuentran en el área indicada.
     */
    std::unordered_set<Edificio*> obtener_edificios_en(const Rectangulo& area);

    /**
     * \brief Devuelve las tropas que se encuentran en el área indicada.
     */
    std::unordered_set<Tropa*> obtener_tropas_en(const Rectangulo& area);

    /**
     * \brief Obtiene la celda en la posición dada.
     */
    void obtener_celda(const Posicion& posicion, int& celda_x, int& celda_y);

    /**
     * \brief Devuelve true si se puede construir sobre la celda.
     */
    bool es_construible(int x_celda, int y_celda) const;

    /**
     * \brief Selecciona la unidad que está en la posición indicada.
     * 
     * Si no hubiera unidad se devolverá un puntero nulo.
     * Las coordenadas están dadas en píxeles globales
     */
    Tropa* obtener_tropa_en(const Posicion& posicion);

    /**
     * \brief Devuelve el edificio que se encuentra en la posición indicada.
     * 
     * Si no hubiera un edificio en dicha posición devolverá un puntero nulo.
     * Las coordenadas están dadas en píxeles globales
     */
    Edificio* obtener_edificio_en(const Posicion& posicion);

    /**
     * \brief Agrega una tropa al terreno.
     * 
     * Esta operación es O(1)
     */
    void agregar_tropa(Tropa& tropa);

    /**
     * \brief Actualiza la posición de una tropa que estaba en (x_ant, y_ant)
     *        a su posición actual.
     * 
     * La posición (x_ant, y_ant) está dada en píxeles globales.
     * Esta operación es O(1)
     */
    void mover_tropa(Tropa& tropa, int x_ant, int y_ant);

    /**
     * \brief Elimina una tropa del terreno.
     * 
     * Esta operación es O(1)
     */
    void eliminar_tropa(const Tropa& tropa);

    /**
     * \brief Agrega un nuevo edificio al terreno.
     * 
     * Esta operacion es O(h*b) con h y b las dimensiones del edificio
     * en celdas. Siendo que h y b suelen ser constantes chicas esta operación
     * es normalmente de tiempo constante.
     */
    void agregar_edificio(Edificio& edificio);

    /**
     * \brief Elimina un edificio del terreno.
     * 
     * Esta operacion es O(h*b) con h y b las dimensiones del edificio
     * en celdas. Siendo que h y b suelen ser constantes chicas esta operación
     * es normalmente de tiempo constante.
     */
    void eliminar_edificio(const Edificio& edificio);

    /**
     * \brief Destructor.
     */
    ~Terreno();

private:
    /**
     * \brief Representación del terreno como un arreglo de celdas.
     * 
     * El terreno se representa como un arreglo de celdas de 32x32 píxeles.
     */
    std::vector<std::vector<Celda>> terreno;

    /**
     * \brief Dimensiones del terreno, en celdas.
     */
    int ancho = 0, alto = 0;

    /**
     * \brief Optimización para evitar redibujar el terreno celda por celda
     *        en cada cuadro.
     */
    int ultimo_celda_x0;
    int ultimo_celda_x1;
    int ultimo_celda_y0;
    int ultimo_celda_y1;
    
    /**
     * \brief Itera sobre todas las celdas que están contenidas en el área y
     * para cada una de ellas aplica "accion". Si considerar_overflow es true
     * entonces se agregará una celda hacia abajo y hacia la izquierda.
     * 
     * bool accion(const Posicion& posicion): Callback a ejecutar para cada 
     * celda. Debe devolver true si se quiere seguir iterando o false en caso 
     * contrario.
     */
    void para_cada_celda_en(const Rectangulo& area, 
        std::function<bool(int, int)> accion, 
        bool considerar_overflow = false);
};

} // namespace cliente

#endif // _TERRENO_H_
