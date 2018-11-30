#ifndef _PROCESADOR_CONFIGURACION_H_
#define _PROCESADOR_CONFIGURACION_H_

#include <string>
#include <vector>

#include "libs/json.hpp"

namespace servidor {

/**
 * \brief Procesa la configuración del servidor y provee una interfaz para
 *        obtener los distintos parámetros.
 */
class ProcesadorConfiguracion {
public:
    /**
     * \brief Procesa los argumentos de línea de comando sobreescribiendo
     *        los valores correspondientes de configuración.
     * 
     * argc: Cantidad de argumentos en argv.
     * argv: Argumentos de línea de comandos.
     * 
     * Si hay un error en los argumentos o no se puede abrir el archivo de 
     * configuración lanzará una excepción de tipo ErrorConfiguracion.
     */
    ProcesadorConfiguracion(int argc, char *argv[]);


    /**
     * \brief Devuelve el servicio en el cual el servidor escuchará.
     */
    const std::string& servicio() const;

    /**
     * \brief Devuelve true si el usuario ingresó el parámetro -h
     */
    bool imprimir_ayuda() const;

    /**
     * \brief Obtiene la información sobre los edificios.
     */
    const nlohmann::json& obtener_edificios() const;

    /**
     * \brief Obtiene la información sobre los ejércitos.
     */
    const nlohmann::json& obtener_ejercitos() const;

    /**
     * \brief Devuelve la lista de rutas a mapas disponibles.
     */
    const std::vector<std::string>& obtener_mapas() const;

    /**
     * \brief Muestra la configuración actual por la salida estándar.
     */
    void mostrar_configuracion() const;

private:
    std::string servicio_;
    bool imprimir_ayuda_ = false;
    const char* ruta_archivo_configuracion;

    std::string ruta_edificios, ruta_ejercitos;

    nlohmann::json edificios;
    nlohmann::json ejercitos;
    std::vector<std::string> mapas;

    /**
     * \brief Carga la configuración por defecto.
     */
    void cargar_configuracion_defecto();

    /**
     * \brief Carga la configuración desde el archivo config.json.
     */
    void cargar_configuracion();

    /**
     * \brief Las siguientes funciones son callbacks para procesar los
     *        argumentos de líneas de comandos.
     * 
     * Todos los callbacks toman 3 argumentos:
     * argc y argv exactamente como los recibió la función main,
     * y un tercer argumento "i" que es la posición en la que se encontró su
     * identificador. 
     * 
     * Las funciones deben devolver la posición del último argumento utilizado. 
     * Por ejemplo, si no se utilizó ningún argumento extra (sólo se activó un
     * switch) deberían devolver el mismo valor de "i"; pero si hubieran usado
     * el argumento siguiente deberían devolver "i + 1".
     * 
     * Si el argumento encontrado es inválido lanzarán ErrorConfiguración.
     * Si se quiere detener el procesamiento de argumentos (caso ayuda), se
     * puede devolver el valor de "argc".
     */
    int procesar_argumento_puerto(int argc, char* argv[], int i);
    int procesar_argumento_ayuda(int argc, char* argv[], int i);
    int procesar_argumento_configuracion(int argc, char* argv[], int i);

};

} // namespace servidor

#endif // _PROCESADOR_CONFIGURACION_H_
