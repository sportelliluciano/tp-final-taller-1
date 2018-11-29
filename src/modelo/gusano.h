#ifndef _GUSANO_H_
#define _GUSANO_H_

#define TIEMPO_ESPERA 100000 

namespace modelo {

/**
 * \brief Gusano. 
 * 
 * Representa un gusano en el juego.
 */
class Gusano { 
private:
    int tiempo_espera = TIEMPO_ESPERA;
    int id = -1;//id incompatible con los ids de las demas unidades

public:
    Gusano();
    ~Gusano();
    bool esperando_comer();
    void comer();
    void actualizar(int dt);
    int get_id();
};
} // namespace modelo

#endif // _CELDA_H_
