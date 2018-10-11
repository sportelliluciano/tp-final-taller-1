#ifndef _HILO_H_
#define _HILO_H_

#include <exception>
#include <string>
#include <thread>

/**
 * Encapsula un hilo.
 * 
 * Todos los hilos de esta aplicación deben heredar de esta clase.
 */
class Hilo {
public:
    /* Constructor por defecto */
    Hilo() = default;

    /**
     * Inicia el hilo. Una vez que el hilo fue iniciado, la instancia no puede
     * ser movida.
     */
    void iniciar();

    /**
     * Detiene el hilo. Este método solicita al hilo que debe terminar sus 
     * acciones y luego realiza un join al mismo.
     * 
     * Si el hilo no se detiene podría bloquear la ejecución del programa.
     */
    void detener();

    /**
     * Indica si el hilo está corriendo actualmente.
     */
    bool esta_corriendo() const noexcept;

    /**
     * Realiza un join al hilo.
     */
    void join();

    /**
     * Si se produjo una excepción (termino_correctamente devuelve false) 
     * entonces esta función devuelve un mensaje de error indicando detalles
     * sobre la excepción ocurrida. 
     * La excepción debe ser una instancia de runtime_error para poder ser
     * atrapado el mensaje.
     * Si la excepción es de otro tipo entonces devolverá "Excepción 
     * desconocida".
     * 
     * Si el hilo sigue corriendo o no se produjo ninguna excepción lanzará 
     * runtime_error.
     */
    const std::string& obtener_error() const;

    /**
     * Devuelve true si el hilo terminó correctamente o false si terminó a 
     * causa de una excepción no controlada.
     * 
     * Lanza runtime_error si el hilo sigue corriendo.
     */
    bool termino_correctamente() const;

    /**
     * Destructor.
     * 
     * Realiza un join al hilo si el mismo está corriendo.
     * Este destructor puede bloquear la ejecución si el hilo quedó bloqueado o
     * si el destructor de la clase hija no solicitó la detención del mismo.
     */
    virtual ~Hilo() noexcept;

protected:
    /**
     * Función a ejecutar en el hilo.
     */
    virtual void correr() = 0;

    /**
     * Esta función solicita al hilo que se detenga.
     * 
     * El hilo al recibir esta solicitud debe terminar lo antes posible, debido
     * a que será unido al hilo que solicitó la detención y por consiguiente
     * bloqueará su ejecución.
     * 
     * Este método podría ser llamado aún cuando el hilo no esté corriendo, así
     * que debería contemplar esta situación.
     */
    virtual void solicitar_detencion() = 0;

private:
    std::thread hilo;
    std::string msj_error;
    bool fue_iniciado = false, hilo_termino = false, hubo_excepcion = false;

    /* Deshabilitar copia */
    Hilo(const Hilo& otro) = delete;
    Hilo& operator=(const Hilo& otro) = delete;

    /**
     * Wrapper para correr el hilo y capturar las excepciones que puedan 
     * producirse durante la ejecución del mismo.
     */
    void wrapper_correr();
};

#endif // _HILO_H_
