#include "test/testing.h"

#include <sstream>
#include <string>

#include "modelo/terreno.h"

class TestTerrenoAEstrella : public Testeable {
public:
    const char *obtener_nombre_set() {
        return "Terreno: A-Estrella";
    }

    void correr_tests() override {
        CORRER(test_encuentra_camino_minimo_recto);
        CORRER(test_encuentra_camino_minimo_obstaculo_direccion_opuesta);
        CORRER(test_encuentra_camino_minimo_obstaculo_al_llegar);
    }

private:
    std::string camino_a_str(const std::vector<modelo::Posicion>& camino) {
        std::stringstream str_camino;
        for (size_t i=0; i<camino.size(); i++) {
            char buffer[50];
            int c = snprintf(buffer, 50, "(%.1f, %.1f) > ", camino[i].x(), 
                camino[i].y());
            
            if (i == camino.size() - 1)
                buffer[c - 2] = ']';
            
            str_camino << buffer;
        }

        return str_camino.str();
    }

    void verificar_igualdad_camino(std::vector<modelo::Posicion> obtenido, 
        std::vector<modelo::Posicion> esperado) {
        if (obtenido.size() != esperado.size()) {
            error("La cantidad de pasos no coincide [obt: %d] != [esp: %d]\n"
                "> Camino obtenido: %s\n"
                "< Camino esperado: %s", obtenido.size(), esperado.size(),
                camino_a_str(obtenido).c_str(), camino_a_str(esperado).c_str());
        }

        for (size_t i=0;i<obtenido.size();i++) {
            if (obtenido[i] != esperado[i]) {
                error("El camino difiere a partir del paso %d:\n"
                  "> Camino obtenido: %s\n"
                  "< Camino esperado: %s", i + 1,
                   camino_a_str(obtenido).c_str(), 
                   camino_a_str(esperado).c_str());
            }
        }
    }
    void test_encuentra_camino_minimo_recto() {
        using namespace modelo;
        Terreno terreno(10, 10);
        terreno.obtener_celda(0, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(1, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(2, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 4).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 5).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 7).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 8).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 9).set_tipo(0); // Bloqueo

        // Caso 1: Línea recta
        std::vector<Posicion> resultado = terreno.buscar_camino_minimo(
            terreno.obtener_celda(0, 4), terreno.obtener_celda(0, 9));
        
        std::vector<Posicion> resultado_esperado = {
            Posicion(0, 5), Posicion(0, 6), Posicion(0, 7), Posicion(0, 8), 
            Posicion(0, 9) };
        
        verificar_igualdad_camino(resultado, resultado_esperado);
    }

    void test_encuentra_camino_minimo_obstaculo_direccion_opuesta() {
        // Este test depende de muchos factores. La heurística es uno de 
        //  ellos. Para armarlo me basé en este sitio:
        // http://qiao.github.io/PathFinding.js/visual/
        using namespace modelo;
        Terreno terreno(10, 10);
        terreno.obtener_celda(0, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(1, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(2, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 4).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 5).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 7).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 8).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 9).set_tipo(0); // Bloqueo

        std::vector<Posicion> resultado = terreno.buscar_camino_minimo(
            terreno.obtener_celda(0, 4), terreno.obtener_celda(7, 1));
        
        std::vector<Posicion> resultado_esperado = {
            Posicion(1, 5), Posicion(2, 6), Posicion(3, 6), Posicion(4, 6),
            Posicion(5, 5), Posicion(5, 4), Posicion(6, 3), Posicion(6, 2),
            Posicion(7, 1)
        };
        
        verificar_igualdad_camino(resultado, resultado_esperado);
    }

    void test_encuentra_camino_minimo_obstaculo_al_llegar() {
        // Este test depende de muchos factores. La heurística es uno de 
        //  ellos. Para armarlo me basé en este sitio:
        // http://qiao.github.io/PathFinding.js/visual/
        // Usar heuristica euclídea, permitir diagonales, no cruzar esquinas.
        using namespace modelo;
        Terreno terreno(10, 10);
        terreno.obtener_celda(0, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(1, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(2, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 3).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 4).set_tipo(0); // Bloqueo
        terreno.obtener_celda(3, 5).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 7).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 8).set_tipo(0); // Bloqueo
        terreno.obtener_celda(7, 9).set_tipo(0); // Bloqueo

        std::vector<Posicion> resultado = terreno.buscar_camino_minimo(
            terreno.obtener_celda(0, 4), terreno.obtener_celda(8, 8));
        
        std::vector<Posicion> resultado_esperado = {
            Posicion(1, 5), Posicion(2, 6), Posicion(3, 6), Posicion(4, 6),
            Posicion(5, 5), Posicion(6, 4), Posicion(6, 3), Posicion(6, 2),
            Posicion(7, 1)
        };
        
        verificar_igualdad_camino(resultado, resultado_esperado);
    }
};

AGREGAR_TEST(TestTerrenoAEstrella);
