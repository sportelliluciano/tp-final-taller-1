#ifndef _GUSANO_H_
#define _GUSANO_H_

#define TIEMPO_ESPERA 150000//2.5min

namespace modelo {

/**
 * \brief Gusano. 
 * 
 * Representa un gusano en el juego.
 * Tiempo_espera: tiempo que espera el gusano para comer.
 * id: identificador unico para el gusano.
 */
class Gusano { 
private:
    int tiempo_espera = TIEMPO_ESPERA;
    int id = -1;//id incompatible con los ids de las demas unidades

public:
    Gusano();
    ~Gusano();
    /**
     * \brief Indica si ya paso el tiempo necesario para comer.
     */
    bool esperando_comer();
    /**
     * \brief Resetea el contaor de tiempo para comer.
     */
    void comer();
    /**
     * \brief Actualizacion temporal para comer.
     */
    void actualizar(int dt);
    int get_id();
};
} // namespace modelo

#endif // _GUSANO_H_
