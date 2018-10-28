#include "cliente/video/log.h"

#include <cstdio>
#include <cstdarg>

#include <string>

#define COLOR_ROJO      "\x1b[31m"
#define COLOR_VERDE     "\x1b[32m"
#define COLOR_AMARILLO  "\x1b[33m"
#define COLOR_RESET     "\x1b[0m"

namespace cliente {

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
 * Función ayudante para escribir un mensaje con un color a la terminal y luego
 * restaurar el color por defecto.
 */
static void escribir(const char *color, const char *archivo, int linea, 
    const char *funcion, const char *fmt, va_list args) 
{
    printf("%s[%s:%d > %s] %s", color, basename(archivo), linea, funcion,
        COLOR_RESET);
    vprintf(fmt, args);
    printf("\n");
}

Log::Log() { }

Log& Log::obtener_instancia() {
    static Log log;
    return log;
}

void Log::dbg(const char* archivo, int linea, const char* funcion, 
    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    escribir(COLOR_VERDE, archivo, linea, funcion, fmt, args);
    va_end(args);
}

void Log::warn(const char* archivo, int linea, const char* funcion, 
    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    escribir(COLOR_AMARILLO, archivo, linea, funcion, fmt, args);
    va_end(args);
}


void Log::err(const char* archivo, int linea, const char* funcion, 
    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    escribir(COLOR_ROJO, archivo, linea, funcion, fmt, args);
    va_end(args);
}

} // namespace cliente
