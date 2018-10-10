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

    bool correr_tests() override {
        // TODO
        CORRER(test_true_es_true);
        return true;
    }

    void teardown() override { }

private:
    // TODO
    bool test_true_es_true() {
        return assert(true == true, "FUCK TRUE NO ES TRUE", 0);
    }
};

// TODO
AGREGAR_TEST(TestPlantilla);