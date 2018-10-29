#ifndef _TERRENO_H_
#define _TERRENO_H_

#include <vector>

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
    Terreno(const char *ruta_csv);

    /**
     * \brief Devuelve la celda en la posición (x, y).
     * 
     * Devuelve la celda en la posición (x, y) donde x e y son la posición
     * en celdas.
     */
    const Celda& at(int x, int y) const;

    /**
     * \brief Devuelve el alto del terreno.
     */
    int alto() const;

    /**
     * \brief Devuelve el ancho del terreno.
     */
    int ancho() const;
    
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
     * \brief Devuelve true si alguna celda está en la cámara actual.
     */
    bool esta_en_camara(std::vector<Celda> celdas, Ventana& ventana);
    
    /**
     * \brief Devuelve las celdas visibles.
     */
    std::vector<Celda> obtener_celdas_visibles(Ventana& ventana);

    /**
     * \brief Convierte las coordenadas de celdas en píxeles.
     * 
     * Convierte un punto (x, y) dado en celdas en su posición en la ventana
     * en píxeles. Tiene en consideración la posición de la cámara.
     */
    void convertir_a_px(int x, int y, int& x_px, int& y_px) const;

    /**
     * \brief Destructor.
     */
    ~Terreno();

private:
    std::vector<std::vector<Celda>> terreno;
    int ancho_, alto_;
    int camara_x = 0, camara_y = 0;
    bool is_dirty = true;
    Textura textura_cacheada;

    void renderizar_mosaico(Ventana& ventana, int x, int y, Textura& destino);
};

} // namespace cliente

#endif // _TERRENO_H_
