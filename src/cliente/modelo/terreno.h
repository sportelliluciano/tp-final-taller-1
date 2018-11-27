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
    Terreno(const nlohmann::json& mapa);
    
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
     * 
     * En caso de pasarse una posición se devolverán las tropas que se 
     * encuentran en el rectángulo de 8x8 con centro en la posición.
     */
    std::unordered_set<Tropa*> obtener_tropas_en(const Rectangulo& area);
    std::unordered_set<Tropa*> obtener_tropas_en(const Posicion& punto);

    /**
     * \brief Obtiene la celda en la posición dada.
     */
    void obtener_celda(const Posicion& posicion, int& celda_x, int& celda_y);

    /**
     * \brief Devuelve true si se puede construir sobre la celda.
     */
    bool es_construible(int x_celda, int y_celda) const;

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
    void mover_tropa(Tropa& tropa, const Posicion& anterior);

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
     * \brief Devuelve el ancho del terreno, en píxeles.
     */
    int obtener_ancho_px() const;

    /**
     * \brief Devuelve el alto del terreno, en píxeles.
     */
    int obtener_alto_px() const;

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
     * bool accion(int celda_x, int celda_y, bool contenida_totalmente): 
     * Callback a ejecutar para cada celda. Los parámetros celda_x y celda_y
     * indican cual es la celda sobre la que se está ejecutando la acción.
     * El parámetro contenida_totalmente indica si la celda está contenida 
     * totalemte por el rectángulo o si es un borde (está contenida 
     * parcialemente). 
     * Si el parámetro contenida_totalmente es false podría darse que la
     * celda no esté contenida en absoluto.
     * La función debe devolver true si se quiere seguir iterando sobre el 
     * resto de las celdas o false para detener la iteración inmediatamente.
     */
    void para_cada_celda_en(const Rectangulo& area, 
        std::function<bool(int, int, bool)> accion);
};

} // namespace cliente

#endif // _TERRENO_H_
