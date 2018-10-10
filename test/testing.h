#ifndef _TESTING_H_
#define _TESTING_H_

/**
 * Esta clase define algunas clases utilitarias como framework de testing.
 * Lo único realmente relevante es la macro AGREGAR_TEST, que debe estar al 
 * final de cada archivo de pruebas unitarias (ver test/test-a.h para 
 * un ejemplo de prueba unitaria).
 */

/**
 * Macro error(formato, varargs)
 * Indica que la prueba que estaba corriendo falló.
 * formato: cadena tipo printf
 * vargars: argumentos tipo printf.
 */
#define error(fmt, ...) lanzar_error(__LINE__, __func__, __FILE__, (fmt), ##__VA_ARGS__)

/**
 * Macro assert(condicion, formato, varargs)
 * Evalúa condición e indica que la prueba falló si condición es falsa.
 * 
 * condicion: cualquier cosa que se pueda meter en un if
 * formato: cadena tipo printf
 * varargs: argumentos tipo printf
 */ 
#define assert(cond, fmt, ...) {if (!(cond)) { error(fmt, ##__VA_ARGS__); }}

/**
 * Macro int_assert(a, b, q)
 * Atajo a assert para tipos enteros, si a != b indica que la función falló e
 * imprime un mensaje que indica qué falló (parámetro q) y el valor de a y b.
 * 
 * a: Primer entero a comparar
 * b: Segundo entero a comparar
 * q: Cadena indicando qué se esta comparando (normalmente nombre de método).
 */
#define int_assert(a, b, q) { assert((a) == (b), q " [%d] != [%d]", (a), (b)); }

/**
 * Macro no_lanza_excepcion(expr, q)
 * Ejecuta expresión e indica que la prueba falló si la prueba lanza una 
 * excepción.
 * 
 * expr: código entre { }
 * q: Cadena indicando qué falló.
 * 
 * Ejemplo:
 * no_lanza_excepcion({ ClasePrueba p = p(); }, "Constructor ClasePrueba")
 */
#define no_lanza_excepcion(expr, q) { \
    try { expr } catch(...){ error("%s", (q)); } \
}

/**
 * Macro lanza_excepcion
 * Ejecuta la expresión esperando que lance un tipo particular de excepción.
 * Si la expresión no lanza dicha excepción, indica que la prueba falló.
 * 
 * expr: código entre { }
 * ex: tipo de la excepción, sin modificadores. ej: std::runtime_error
 * q: Cadena indicando qué falló.
 * 
 * Ej:
 * lanza_excepcion({ server.enviar(-1); }, ErrorValorInvalido, "server.enviar")
 */
#define lanza_excepcion(expr, ex, q) {\
    try { expr } catch (const ex& e) {} catch(...) { \
    error("[%s] Lanzó un tipo de excepción diferente a "##ex" ", q); } \
    error("[%s] No lanzó ninguna excepción", q);}

/**
 * Macro AGREGAR_TEST(x)
 * 
 * Agrega el test a la lista de tests a ejecutar.
 */
#define AGREGAR_TEST(x) static x test_##x = x()

/**
 * Macro CORRER(x)
 * Corre el test imprimiendo un mensaje con el nombre y el estado.
 */
#define CORRER(x) {\
    std::cerr << ">> Ejecutando " #x ": "; \
    x();\
    std::cerr << "OK" << std::endl;\
}

#include <string>
#include <sstream>
#include <exception>
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <string>
#include <list>

void tirar_error(int linea, std::string test, std::string filename, const char *formato, ...);

/**
 * Clase Testeable. Todos los tests deben heredar de esta clase.
 */
class Testeable {
public:
    /** No sobre-escribir el constructor por defecto. **/
    Testeable();

    /** Lo que iria en el constructor meterlo en setup. **/
    virtual void setup();

    /**
     * Devuelve el nombre del set de test.
     */
    virtual const char* obtener_nombre_set() = 0;

    /**
     * Wrapper para correr las pruebas y capturar cualquier excepción que
     * se produzca.
     * 
     * Devuelve true si el set de pruebas se ejecuto sin errores.
     */
    bool ejecutar_set_test();

    /** Lo que iria en el destructor meterlo en teardown. **/
    virtual void teardown();

protected:
    /**
     * Ejecuta todas las pruebas del test.
     * Debe lanzar una excepción de tipo runtime_error si alguna prueba del
     * set falla. Utilizar macro error() para esto.
     */
    virtual void correr_tests() = 0;

    /**
     * Lanza un std::runtime_error con información sobre el test que falló.
     * No debería ser usado directamente, utilizar macro error() para esto.
     */
    void lanzar_error(int linea, const std::string& test,
        const std::string& archivo, const char *formato, ...);
};

/**
 * Singleton para almacenar los tests a correr
 */
class Testing {
private:
    Testing() = default;
    std::list<Testeable*> tests_a_correr;
public:
    static Testing& obtener_instancia();
    void agregar_test(Testeable& test);
    bool correr_tests();
};


#endif // _TESTING_H_