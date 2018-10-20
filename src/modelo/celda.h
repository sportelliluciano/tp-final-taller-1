#ifndef _CELDA_H_
#define _CELDA_H_

namespace modelo {

/**
 * \brief Celda del terreno. 
 * 
 * Representa una unidad mínima del terreno.
 */
class Celda { 
public:
    /**
     * \brief Constructor por defecto.
     */
    Celda() = default;
    Celda(int x_, int y_);

    int tipo() const;
    int x() const;
    int y() const;

    void set_tipo(int tipo);

    bool es_caminable() const;

private:
    int posicion_x, posicion_y;
    int tipo_celda;
};

} // namespace modelo

#endif // _CELDA_H_
