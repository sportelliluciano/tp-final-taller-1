#include "comun/log.h"

#include <cstdio>
#include <cstdarg>

#include <string>

#define COLOR_ROJO      "\x1b[31m"
#define COLOR_VERDE     "\x1b[32m"
#define COLOR_AMARILLO  "\x1b[33m"
#define COLOR_RESET     "\x1b[0m"

/**
 * Función ayudante para recortar la ruta al archivo.
 * Convierte /ruta/muy/larga/de/archivo.cpp en archivo.cpp
 */
static inline const char *basename(const char *archivo) {
    int ultima_barra = 0;
    for (int i=0; archivo[i] != 0; i++) {
        if ((archivo[i] == '/') || (archivo[i] == '\\'))
            ultima_barra = i + 1;
    }

    return archivo + ultima_barra;
}

/**
 * \brief Devuelve el timestamp actual desde que inició el sistema de logging
 *        en milisegundos.
 */
static int64_t timestamp() {
    using namespace std::chrono;
    static auto base = steady_clock::now();
    return duration_cast<milliseconds>(steady_clock::now() - base).count();
}

/**
 * Función ayudante para escribir un mensaje con un color a la terminal y luego
 * restaurar el color por defecto.
 */
static void escribir(const char *color, const char *archivo, int linea, 
    const char *funcion, const char *fmt, va_list args, std::mutex& m_log) 
{
    std::lock_guard<std::mutex> lock(m_log);
    printf("%s[%s:%d > %s] %s", color, basename(archivo), linea, funcion,
        COLOR_RESET);
    vprintf(fmt, args);
    printf("\n");
}

Log::Log() {
    // Inicializar el timestamp.
    timestamp(); 
}

Log& Log::obtener_instancia() {
    static Log log;
    return log;
}

void Log::dbg(const char* archivo, int linea, const char* funcion, ...)
{
    va_list args;
    va_start(args, funcion);
    char *fmt = va_arg(args, char*);
    escribir(COLOR_VERDE, archivo, linea, funcion, fmt, args, m_cout);
    va_end(args);
}

void Log::warn(const char* archivo, int linea, const char* funcion, ...) {
    va_list args;
    va_start(args, funcion);
    const char* fmt = va_arg(args, const char*);
    escribir(COLOR_AMARILLO, archivo, linea, funcion, fmt, args, m_cout);
    va_end(args);
}

void Log::err(const char* archivo, int linea, const char* funcion, ...) {
    va_list args;
    va_start(args, funcion);
    const char* fmt = va_arg(args, const char*);
    std::lock_guard<std::mutex> lock(m_cout);
    fprintf(stderr, "%s[%s:%d > %s] %s", COLOR_ROJO, basename(archivo), linea, 
        funcion, COLOR_RESET);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void Log::cout(const char* fmt, ...) {
    std::lock_guard<std::mutex> lock(m_cout);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void Log::cerr(const char* fmt, ...) {
    std::lock_guard<std::mutex> lock(m_cerr);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void Log::red(bool saliente, ...) {
    std::lock_guard<std::mutex> lock(m_cout);
    const char *color = (saliente) ? COLOR_VERDE : COLOR_ROJO;
    const char *direccion = (saliente) ? ">>" : "<<";
    printf("%s[%lu ms] %s%s%s ", COLOR_VERDE, timestamp(), color, direccion, 
        COLOR_RESET);
    va_list args;
    va_start(args, saliente);
    const char* fmt = va_arg(args, const char*);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void Log::tiempo(const char* evento, uint64_t tiempo) {
    std::lock_guard<std::mutex> lock(m_cout);
    printf("%s[%lu ms]%s t(%s) = %lu ms\n", COLOR_VERDE, timestamp(), 
        COLOR_RESET, evento, tiempo);
}