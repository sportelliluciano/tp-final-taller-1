#ifndef _POSICION_H_
#define _POSICION_H_

/**
 * \brief Tolerancia en la comparación de posiciones por igualdad.
 * 
 * Las comparaciones entre floats admiten esta tolerancia para decir que son
 * iguales.
 */
#define POSICION_CMP_TOLERANCIA 1e-5

namespace modelo {

/**
 * \brief Posición en el mundo de juego.
 * 
 * Esta clase representa una posición en el espacio euclídeo de juego.
 * Se implementa como un vector de dos coordenadas.
 */
class Posicion {
public:
    /**
     * \brief Constructor por defecto.
     *
     * Inicializa la posición en (0, 0).
     */
    Posicion();

    /**
     * \brief Constructor desde una posición.
     * 
     * Inicializa la posición en (x, y).
     */
    Posicion(float x, float y);

    /**
     * \brief Sobrecarga de operador +
     * 
     * Realiza la suma componente a componente.
     */
    Posicion operator+(const Posicion& otro) const;

    /**
     * \brief Sobrecarga de operador ==
     * 
     * Realiza la comparación componente a componente.
     * Dado que se utilizan números con punto flotante la comparación se 
     * realiza teniendo en cuenta una tolerancia definida por 
     * POSICION_CMP_TOLERANCIA. Comunmente esta tolerancia es de 10^-5.
     */
    bool operator==(const Posicion& otro) const;
    bool operator!=(const Posicion& otro) const;

    /**
     * \brief Calcula la distancia euclídea de la posición actual a otra 
     *        posición.
     * 
     * Realiza el cálculo de la distancia euclídea de la posición actual
     * a otra posición.
     */
    float distancia_a(const Posicion& otro) const;

    /**
     * \brief Getters para acceder por coordenada.
     * 
     * El uso de estos getters está desaconsejado ya que acoplan el código que
     * los use al espacio dimensional (2D). Sin embargo en algunos lugares es
     * inevitable su utilización y por consiguiente se los provee.
     */
    float x() const;
    float y() const;
    float px_x() const;
    float px_y() const;
    void actualizar(float x,float y);
    void actualizar_px_x(float x);
    void actualizar_px_y(float y);

private:
    float x_ = 0, y_ = 0;
    float px_x_ = 0, px_y_ = 0;
};

} // namespace modelo

#endif // _POSICION_H_
