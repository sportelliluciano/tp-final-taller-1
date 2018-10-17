#include "test/testing.h"

#include "modelo/posicion.h"

class TestPosicion : public Testeable {
public:
    const char *obtener_nombre_set() {
        return "Posicion";
    }

    void setup() override { }

    void correr_tests() override {
        CORRER(test_posicion_por_defecto_es_0_0);
        CORRER(test_comparacion_por_igualdad);
        CORRER(test_comparacion_reflexion);
        CORRER(test_operador_suma);
    }

    void teardown() override { }

private:
    void test_posicion_por_defecto_es_0_0() {
        using modelo::Posicion;

        Posicion posicion;

        assert((posicion.x() == 0.0f) && (posicion.y() == 0.0f), 
            "Posicion: (%.5f, %.5f)", posicion.x(), posicion.y());
    }

    void test_comparacion_por_igualdad() {
        using modelo::Posicion;
        Posicion pos_1(0, 0),
                 pos_2(2, 0),
                 pos_3(0, 3),
                 pos_4(4, 4),
                 pos_11(0, 0),
                 pos_22(2, 0),
                 pos_33(0, 3),
                 pos_44(4, 4);
        
        assert(pos_1 != pos_2, 
            "pos_1: (%.5f, %.5f) == pos_2: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_2.x(), pos_2.y());
        
        assert(pos_1 != pos_3, 
            "pos_1: (%.5f, %.5f) == pos_3: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_3.x(), pos_3.y());
        
        assert(pos_1 != pos_4, 
            "pos_1: (%.5f, %.5f) == pos_4: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_4.x(), pos_4.y());
        
        assert(pos_2 != pos_3, 
            "pos_2: (%.5f, %.5f) == pos_3: (%.5f, %.5f)", 
            pos_2.x(), pos_2.y(), pos_3.x(), pos_3.y());
        
        assert(pos_2 != pos_4, 
            "pos_2: (%.5f, %.5f) == pos_4: (%.5f, %.5f)", 
            pos_2.x(), pos_2.y(), pos_4.x(), pos_4.y());
        
        assert(pos_3 != pos_4, 
            "pos_3: (%.5f, %.5f) == pos_4: (%.5f, %.5f)", 
            pos_3.x(), pos_3.y(), pos_4.x(), pos_4.y());
        
        assert(pos_1 == pos_11, 
            "pos_1: (%.5f, %.5f) != pos_11: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_11.x(), pos_11.y());
        
        assert(pos_2 == pos_22, 
            "pos_2: (%.5f, %.5f) != pos_22: (%.5f, %.5f)", 
            pos_2.x(), pos_2.y(), pos_22.x(), pos_22.y());
        
        assert(pos_3 == pos_33, 
            "pos_3: (%.5f, %.5f) != pos_3: (%.5f, %.5f)", 
            pos_3.x(), pos_3.y(), pos_33.x(), pos_33.y());
        
        assert(pos_4 == pos_44, 
            "pos_4: (%.5f, %.5f) != pos_44: (%.5f, %.5f)", 
            pos_4.x(), pos_4.y(), pos_44.x(), pos_44.y());
    }

    void test_comparacion_por_igualdad_tolerancia() {
        using modelo::Posicion;
        Posicion pos_1(5, 7),
                 pos_1e(5 + 1e-6, 7),
                 pos_e1(5, 7 + 1e-6),
                 pos_1s(5 + 1e-4, 7),
                 pos_s1(5, 7 + 1e-4);
        
        assert(pos_1 == pos_1e, 
            "pos_1: (%.5f, %.5f) != pos_1e: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_1e.x(), pos_1e.y());
        
        assert(pos_1 == pos_e1, 
            "pos_1: (%.5f, %.5f) != pos_e1: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_e1.x(), pos_e1.y());
        
        assert(pos_1 != pos_1s, 
            "pos_1: (%.5f, %.5f) == pos_1s: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_1s.x(), pos_1s.y());
        
        assert(pos_1 != pos_s1, 
            "pos_1: (%.5f, %.5f) == pos_s1: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_s1.x(), pos_s1.y());              
    }

    void test_comparacion_reflexion() {
        using modelo::Posicion;
        Posicion pos_1(5, 7),
                 pos_2(11, 3),
                 pos_3(5, 7);
                
        assert(pos_1 == pos_3, 
            "pos_1: (%.5f, %.5f) != pos_3: (%.5f, %.5f)", 
            pos_1.x(), pos_1.y(), pos_3.x(), pos_3.y());
        
        assert(pos_3 == pos_1, 
            "pos_3: (%.5f, %.5f) != pos_1: (%.5f, %.5f)", 
            pos_3.x(), pos_3.y(), pos_1.x(), pos_1.y());
        
        assert(pos_1 != pos_2, 
            "pos_2: (%.5f, %.5f) == pos_2: (%.5f, %.5f)", 
            pos_2.x(), pos_2.y(), pos_2.x(), pos_2.y());
        
        assert(pos_2 != pos_1, 
            "pos_2: (%.5f, %.5f) == pos_1: (%.5f, %.5f)", 
            pos_2.x(), pos_2.y(), pos_1.x(), pos_1.y());
    }

    void test_operador_suma() {
        using modelo::Posicion;
        Posicion pos_1(1, 2),
                 pos_2(3, 4),
                 resultado(4, 6),
                 tmp;
        
        tmp = pos_1 + pos_2;

        assert(tmp == resultado, 
            "tmp: (%.5f, %.5f) != resultado: (%.5f, %.5f)", 
            tmp.x(), tmp.y(), resultado.x(), resultado.y());
    }
};

AGREGAR_TEST(TestPosicion);