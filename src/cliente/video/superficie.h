#ifndef _SUPERFICIE_H_
#define _SUPERFICIE_H_

#include "cliente/video/administrador_texturas.h"
#include "cliente/video/color.h"
#include "cliente/video/rectangulo.h"

namespace cliente {

/**
 * \brief Superficie renderizable.
 * 
 * Esta clase ofrece primitivas de dibujado (lineas, rectángulos), primitivas
 * de carga y escalado de imagenes y primitivas de renderizado de texto.
 */
class Superficie {
public:
    /**
     * \brief Devuelve true si la superficie cambió y requiere ser redibujada.
     */
    bool requiere_redibujado() const;

    /**
     * \brief Dibuja el contorno de un rectángulo.
     * 
     * El borde del mismo se dibujará hacia afuera.
     */
    void dibujar_rectangulo(const Rectangulo& rc, const Color& color, 
        int ancho_borde);

    /**
     * \brief Dibuja un rectángulo lleno del color indicado.
     */
    void rellenar_rectangulo(const Rectangulo& rc, const Color& color);

    /**
     * \brief Dibuja una línea recta que une los puntos x e y del color
     *        indicado y con el grosor indicado.
     */
    void dibujar_linea(const Posicion& x, const Posicion& y, const Color& color,
        int grosor);
    
    /**
     * \brief Dibuja un sprite sobre la superficie.
     */
    void dibujar_sprite(int id, int x, int y);

    /**
     * \brief Dibuja un sprite sobre la superficie y lo redimensiona al tamaño
     * del rectángulo.
     */
    void dibujar_sprite(int id, const Rectangulo& caja);

    /**
     * \brief Limpia la superficie al color indicado.
     */
    void limpiar(const Color& color);


private:
    Superficie(AdministradorTexturas& texturas, int ancho, int alto);

    AdministradorTexturas& admin_texturas;
    int ancho_, alto_;
};

} // namespace cliente

#endif // _SUPERFICIE_H_
