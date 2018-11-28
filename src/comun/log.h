#ifndef _LOGGING_H_
#define _LOGGING_H_

#define LOG_RED_SALIENTE true
#define LOG_RED_ENTRANTE false

#define log_depuracion(...) \
    Log::obtener_instancia().\
        dbg(__FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_advertencia(...) \
    Log::obtener_instancia().\
        warn(__FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_error(...) \
    Log::obtener_instancia().\
        err(__FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_red(saliente, ...) \
    Log::obtener_instancia().\
        red(saliente, ##__VA_ARGS__)

#define log_tiempo(ev, t) \
    Log::obtener_instancia().\
        tiempo(ev, t)

#include <mutex>

/**
 * \brief Permite registrar acciones
 */
class Log {
public:
    static Log& obtener_instancia();
    
    /**
     * Emite un mensaje de depuración.
     * El primer argumento variable debe ser la cadena con la información de
     * formato estilo printf.
     * Ej: dbg("a.cpp", 10, "funcion", "%s", "test");
     */
    void dbg(const char* archivo, int linea, const char* funcion, ...);

    /**
     * Emite una advertencia
     * El primer argumento variable debe ser la cadena con la información de
     * formato estilo printf.
     * Ej: warn("a.cpp", 10, "funcion", "%s", "test");
     */
    void warn(const char* archivo, int linea, const char* funcion, ...);

    /**
     * Emite un mensaje de error.
     * El primer argumento variable debe ser la cadena con la información de
     * formato estilo printf.
     * Ej: err("a.cpp", 10, "funcion", "%s", "test");
     */
    void err(const char* archivo, int linea, const char* funcion, ...);
    
    /**
     * \brief Emite un mensaje de depuración de red.
     * El primer argumento variable debe ser la cadena con la información de
     * formato estilo printf.
     * Ej: red(LOG_RED_SALIENTE, "{JSON de %d bytes}", 1000);
     */
    void red(bool saliente, ...);

    /**
     * \brief Emite un mensaje de depuración de tiempos.
     */
    void tiempo(const char* evento, uint64_t tiempo);
    
private:
    Log();
    std::mutex m_log;
};

#endif // _LOGGING_H_
