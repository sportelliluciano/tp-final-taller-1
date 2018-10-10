/**
 * Archivo de ejemplo de pruebas unitarias para la clase A.
 * 
 * Los archivos de pruebas unitarias deben componerse únicamente de archivos .h
 * y estar guardados en test/{ejecutable}/test_nombre_de_clase.h
 * 
 * El nombre no es obligatorio para que cmake lo detecte, es solo por 
 * consistencia.
 * 
 * Toda la salida de mensajes se hace por STDERR. La razón para esto es que 
 * stderr (a diferencia de stdout) no está buffereada, con lo cual si llegara a
 * explotar violentamente el programa toda la salida queda en la consola. 
 * 
 * La idea de este microframework es justamente que sea lo mínimo e 
 * indispensable para poder armar las pruebas unitarias que sean necesarias
 * sin tener que luchar demasiado con el código. Agregar pruebas para una clase
 * implican únicamente copiar el archivo test/test-plantilla.h a donde 
 * corresponda, cambiar algunos nombres y compilar.
 */

/** Incluir framework. Provee Testeable, AGREGAR_TEST y otros macros útiles **/
#include "test/testing.h"

/** Incluir la clase a testear, ruta relativa a src. **/
#include "cliente/a.h"

/** 
 * Declarar la clase de prueba como TestNombreDeClase. Nuevamente, no es 
 * obligatorio el nombre pero ayuda a evitar conflictos con clases existentes
 * [las unitarias se linkean contra **todo** el ejecutable] y consistencia.
 * La misma debe heredar de Testeable y **no tener constructor** para que sea
 * levantada por el framework.
 */
class TestA : public Testeable {
public:

    /**
     * Requerido. Devuelve una cadena que identifica el set de tests.
     */ 
    const char *obtener_nombre_set() {
        return "A";
    }

    /**
     * Opcional. Permite ejecutar código antes de iniciar el set de test.
     */
    void setup() override {
        std::cerr << "Hola desde el setup de TestA" << std::endl;
    }

    /**
     * Requerido. 
     * Debe correr todas las pruebas unitarias de la clase.
     * Lanza std::runtime_error si alguna prueba falla.
     */
    void correr_tests() override {
        /**
         * CORRER(x) es una macro definida en testing.h que imprime por pantalla
         * el nombre del test y el resultado del mismo.
         * Por consistencia debería usarse para ejecutar cada test.
         */
        CORRER(test_es_construible);
        CORRER(test_metodo_devuelve_10);
        CORRER(test_prueba_que_falla);
        CORRER(test_prueba_que_nunca_se_va_a_ejecutar_porque_fallo_la_anterior);
    }

    /**
     * Opcional. Permite ejecutar código despues de que todas las pruebas del
     * set de test se ejecutaron.
     */
    void teardown() override {
        std::cerr << "Adios desde el setup de TestA" << std::endl;
    }

private:
    /**
     * Implementación de cada test unitario de la clase.
     */

    void test_es_construible() {
        /**
         * no_lanza_excepcion({ expr }, mensaje_error) es otra macro definida
         * en testing.h que ejecuta expresión y emite un error si la expresión
         * lanzó una excepción.
         */
        no_lanza_excepcion({A a();}, "Constructor de A lanzó excepción");
    }

    void test_metodo_devuelve_10() {
        A a;
        /**
         * int_assert(int_a, int_b, nombre_metodo) es otra macro que permite
         * comparar por igualdad dos enteros y en caso de fallar indicar
         * con un mensaje el método que falló y qué valor tenía.
         */
        int_assert(a.metodo(), 10, "a.metodo()");
    }

    void test_prueba_que_falla() {
        A a;
        int_assert(a.metodo(), 20, "a.metodo()");
    }

    void test_prueba_que_nunca_se_va_a_ejecutar_porque_fallo_la_anterior() {
        std::cerr << "Fuck?" << std::endl;
    }
};

/**
 * Esta última línea instancia la clase y la agrega al set de pruebas a 
 * ejecutar. Si esta línea no está el test no se ejecutará.
 */
AGREGAR_TEST(TestA);