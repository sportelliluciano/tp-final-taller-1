#ifndef _INFRAESTRUCTURA_H_
#define _INFRAESTRUCTURA_H_

#include <unordered_map>

/**
 * \brief 
 * 
 *
 *
 */

namespace modelo {

/**
 * \brief 
 */
class Infraestructura{
    private:
    std::unordered_map<edificio> edificio;
    unsigned int centro_de_construccion = 0;
    unsigned int fabrica_ligera = 0;
    unsigned int fabrica_pesada = 0;
    unsigned int palacio = 0;
    unsigned int silo = 0;
    unsigned int cuartel = 0;
    unsigned int trampa_de_aire = 0;

    public:
    void Crear(int x, int y, char id);
    void crear_centro_de_construccion(int x, int y);
    void crear_fabrica_ligera(int x, int y);
    void crear_fabrica_pesada(int x, int y);
    void crear_palacio(int x, int y);
    void crear_silo(int x, int y);
    void crear_cuartel(int x, int y);
    void crear_trampa_de_aire(int x, int y);
    void destruir_centro_de_construccion(char id);
    void destruir_fabrica_ligera(char id);
    void destruir_fabrica_pesada(char id);
    void destruir_palacio(char id);
    void destruir_silo(char id);
    void destruir_cuartel(char id);
    void destruir_trampa_de_aire(char id);
    bool centro_de_construccion();
    bool fabrica_ligera();
    bool fabrica_pesada();
    bool palacio();
    bool silo();
    bool cuartel();
    bool trampa_de_aire();
};
}
#endif