#include "test/testing.h"

// TODO
#include "cliente/plantilla.h"

class TestPlantilla : public Testeable {
public:
    const char *obtener_nombre_set() {
        // TODO
        return "Plantilla";
    }

    void setup() override { }

    void correr_tests() override {
        // TODO
        CORRER(test_true_es_true);
    }

    void teardown() override { }

private:
    // TODO
    void test_true_es_true() {
        assert(true == true, "FUCK TRUE NO ES TRUE", 0);
    }
};

// TODO
AGREGAR_TEST(TestPlantilla);
