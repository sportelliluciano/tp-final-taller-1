#ifndef _LOGGING_H_
#define _LOGGING_H_

#define log_depuracion(fmt, ...) \
    Log::obtener_instancia().\
        dbg(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define log_advertencia(fmt, ...) \
    Log::obtener_instancia().\
        warn(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define log_error(fmt, ...) \
    Log::obtener_instancia().\
        err(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

namespace cliente {

/**
 * \brief Permite registrar acciones
 */
class Log {
public:
    static Log& obtener_instancia();
    
    /**
     * Emite un mensaje de depuración.
     */
    void dbg(const char* archivo, int linea, const char* funcion, 
        const char *fmt, ...);

    /**
     * Emite una advertencia
     */
    void warn(const char* archivo, int linea, const char* funcion, 
        const char *fmt, ...);

    /**
     * Emite un mensaje de error.
     */
    void err(const char* archivo, int linea, const char* funcion, 
        const char *fmt, ...);
    
private:
    Log();
};

} // namespace cliente

#endif // _LOGGING_H_
