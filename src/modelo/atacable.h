#ifndef _ATACABLE_H_
#define _ATACABLE_H_


/**
 * \brief Interfaz servidor-modelo
 */
class Atacable {
public:
    /**
     * \brief Inicializa el modelo con el mapa, infraestructura y ejército
     *        indicados.
     * 
     * Este método será llamado antes que cualquier otro método.
     */
    virtual int recibir_dano(unsigned int dano) = 0;
    virtual int get_id() = 0;
    /**
     * \brief Destructor virtual.
     */
    virtual ~Atacable() { }
};

#endif 