#ifndef _TERRENO_H_
#define _TERRENO_H_

#include <vector>
#include <unordered_set>

#include "cliente/modelo/celda.h"
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
    void renderizar(Ventana& ventana);

    /**
     * \brief Setea la ubicación de la cámara sobre el terreno.
     * 
     * Esta función permite establecer en que celda está la esquina superior
     * izquierda de la pantalla. Observar que los valores son en unidades
     * (celdas) del terreno y no en pixeles.
     * 
     * La función admite valores negativos y más grandes que el terreno, las 
     * posiciones que no representen una celda del terreno se renderizaran en
     * negro.
     */
    void set_camara(int camara_x_, int camara_y_);

    /**
     * \brief Desplaza la cámara actual en (dx, dy).
     * 
     * Si (dx, dy) == (0, 0) esta operación no tiene ningún efecto.
     * Si la cámara está en algún límite la operación no tiene efecto
     * sobre dicho límite.
     */
    void mover_camara(int dx, int dy);

    /**
     * \brief Devuelve los elementos que son visibles bajo la cámara actual.
     */
    std::unordered_set<const Edificio*> 
        obtener_edificios_visibles(Ventana& ventana);
    std::unordered_set<const Tropa*> 
        obtener_tropas_visibles(Ventana& ventana);

    /**
     * \brief Obtiene la posición en píxeles relativos a la ventana del 
     *        objeto en cuestión según la posición de la cámara.
     */
    void obtener_posicion_visual(const Tropa& tropa, int& x_px, int& y_px);
    void obtener_posicion_visual(const Edificio& edificio, int& x_px, 
        int& y_px);
    
    /**
     * \brief Convierte la celda a su posición en píxeles según la posición de 
     *        la cámara.
     */
    void convertir_a_px(int x_celda, int y_celda, int& x_px, int& y_px);

    /**
     * \brief Selecciona las unidades que estén contenidas en el rectángulo con 
     *        esquinas opuestas en (x0, y0) y (x1, y1).
     * 
     * Las coordenadas están dadas en píxeles relativos a la ventana.
     */
    std::unordered_set<const Tropa*>
        seleccionar_unidades(int x0, int y0, int x1, int y1);

    /**
     * \brief Devuelve el edificio que se encuentra en la posición (x, y).
     * 
     * Si no hubiera un edificio en dicha posición devolverá un puntero nulo.
     * Las coordenadas están dadas en píxeles relativos a la ventana.
     */
    const Edificio* obtener_edificio_en(int x, int y);

    /**
     * \brief Agrega una tropa al terreno.
     * 
     * Esta operación es O(1)
     */
    void agregar_tropa(const Tropa& tropa);

    /**
     * \brief Actualiza la posición de una tropa que estaba en (x_ant, y_ant)
     *        a su posición actual.
     * 
     * La posición (x_ant, y_ant) está dada en píxeles globales.
     * Esta operación es O(1)
     */
    void mover_tropa(const Tropa& tropa, int x_ant, int y_ant);

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
    void agregar_edificio(const Edificio& edificio);

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
     * \brief Posición de la cámara sobre el terreno.
     */
    int camara_x = 0, camara_y = 0;
    int ultimo_camara_x = 0, ultimo_camara_y = 0;

    /**
     * \brief Optimización para evitar redibujar el terreno celda por celda
     *        en cada cuadro.
     */
    bool is_dirty = true;

    bool ultimo_is_dirty = true;
};

} // namespace cliente

#endif // _TERRENO_H_
