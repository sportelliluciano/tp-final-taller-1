#include "servidor/procesador_configuracion.h"

#include <string>
#include <functional>
#include <unordered_map>
#include <fstream>

#include "libs/json.hpp"

#include "comun/log.h"
#include "servidor/config.h"
#include "servidor/error_configuracion.h"

#define ID_PUERTO "puerto"
#define ID_RUTA_EDIFICIOS "ruta_edificios"
#define ID_RUTA_EJERCITOS "ruta_ejercitos"
#define ID_MAPAS "mapas"

#define PUERTO_DEFECTO "9432"
#define RUTA_EDIFICIOS_DEFECTO RUTA_BASE_CONFIG "/edificios.json"
#define RUTA_EJERCITO_DEFECTO RUTA_BASE_CONFIG "/ejercitos.json"
#define MAPAS_DEFECTO {}

namespace servidor {

int ProcesadorConfiguracion::procesar_argumento_puerto(int, char *argv[], 
    int i) 
{
    if (!argv[i+1])
        throw ErrorConfiguracion("Se debe ingresar un puerto o servicio");
    
    servicio_ = argv[i+1];
    return i + 1;
}

int ProcesadorConfiguracion::procesar_argumento_ayuda(int argc, char *[], int) {
    imprimir_ayuda_ = true;
    return argc;
}

int ProcesadorConfiguracion::procesar_argumento_configuracion(
    int, char *argv[], int i) 
{
    if (!argv[i+1]) {
        throw ErrorConfiguracion("Se debe indicar la ruta al archivo de "
            "configuración");
    }
    ruta_archivo_configuracion = argv[i+1];
    return i + 1;
}

void ProcesadorConfiguracion::cargar_configuracion_defecto() {
    ruta_archivo_configuracion = RUTA_CONFIG_JSON;
    servicio_ = "";
    imprimir_ayuda_ = false;
}

ProcesadorConfiguracion::ProcesadorConfiguracion(int argc, char *argv[]) {
    cargar_configuracion_defecto();

    std::unordered_map<std::string, std::function<int(ProcesadorConfiguracion*, 
        int, char*[], int)>> args;

    args.insert({"-p", 
        &ProcesadorConfiguracion::procesar_argumento_puerto});
    args.insert({"--puerto", 
        &ProcesadorConfiguracion::procesar_argumento_puerto});
    args.insert({"-h", 
        &ProcesadorConfiguracion::procesar_argumento_ayuda});
    args.insert({"--help", 
        &ProcesadorConfiguracion::procesar_argumento_ayuda});
    args.insert({"-c", 
        &ProcesadorConfiguracion::procesar_argumento_configuracion});
    args.insert({"--config", 
        &ProcesadorConfiguracion::procesar_argumento_configuracion});
    
    for (int i = 1; i < argc; i++) {
        if (args.count(argv[i]) == 0) {
            throw ErrorConfiguracion(std::string("Parámetro no reconocido: ") + 
                argv[i]);
        }

        std::function<int(ProcesadorConfiguracion*, int, char*[], int)>& 
            callback = args.at(argv[i]);
        i = callback(this, argc, argv, i);
    }

    cargar_configuracion();
}

static std::string desrelativizar_ruta(const std::string& ruta) {
    if ((ruta.length() > 0) && (ruta[0] != '/'))
        return std::string(RUTA_BASE_CONFIG) + "/" + ruta;
    
    return ruta;
}

void ProcesadorConfiguracion::cargar_configuracion() {
    std::ifstream archivo_config(ruta_archivo_configuracion);

    if (!archivo_config.good()) {
        throw ErrorConfiguracion(std::string("No se pudo abrir ") + 
            ruta_archivo_configuracion);
    }

    // Cargar configuración por defecto
    nlohmann::json configuracion = { 
        {ID_PUERTO, PUERTO_DEFECTO},
        {ID_RUTA_EDIFICIOS, RUTA_EDIFICIOS_DEFECTO},
        {ID_RUTA_EJERCITOS, RUTA_EJERCITO_DEFECTO},
        {ID_MAPAS, MAPAS_DEFECTO},
    };
    
    try {
        nlohmann::json config_json;
        archivo_config >> config_json;

        configuracion.update(config_json);

        if (servicio_ == "")
            servicio_ = configuracion.at(ID_PUERTO);
        ruta_edificios = configuracion.at(ID_RUTA_EDIFICIOS);
        ruta_ejercitos = configuracion.at(ID_RUTA_EJERCITOS);
        mapas = configuracion.at(ID_MAPAS).get<std::vector<std::string>>();
    } catch (const std::exception& e) {
        throw ErrorConfiguracion("El archivo de configuración es inválido");
    }

    // Des-relativizar rutas
    for (std::string& mapa : mapas) {
        mapa = desrelativizar_ruta(mapa);
    }

    ruta_edificios = desrelativizar_ruta(ruta_edificios);
    std::ifstream archivo_edificios(ruta_edificios);
    if (!archivo_edificios.good()) {
        throw ErrorConfiguracion("No se pudo abrir: " + ruta_edificios);
    }

    archivo_edificios >> edificios;

    ruta_ejercitos = desrelativizar_ruta(ruta_ejercitos);
    std::ifstream archivo_ejercitos(ruta_ejercitos);
    if (!archivo_ejercitos.good()) {
        throw ErrorConfiguracion("No se pudo abrir: " + ruta_ejercitos);
    }

    archivo_ejercitos >> ejercitos;
}

bool ProcesadorConfiguracion::imprimir_ayuda() const {
    return imprimir_ayuda_;
}

const std::string& ProcesadorConfiguracion::servicio() const {
    return servicio_;
}

const nlohmann::json& ProcesadorConfiguracion::obtener_edificios() const {
    return edificios;
}

const nlohmann::json& ProcesadorConfiguracion::obtener_ejercitos() const {
    return ejercitos;
}

const std::vector<std::string>& ProcesadorConfiguracion::obtener_mapas() const {
    return mapas;
}

void ProcesadorConfiguracion::mostrar_configuracion() const {
    Log& logger = Log::obtener_instancia();
    logger.cout("Configuración cargada desde %s\n", ruta_archivo_configuracion);
    logger.cout("Información de edificios tomada desde: %s\n", 
        ruta_edificios.c_str());
    logger.cout("Información de tropas tomada desde: %s\n", 
        ruta_ejercitos.c_str());
    logger.cout("Mapas cargados: \n");
    for(const std::string& mapa: mapas) {
        logger.cout("\t%s\n", mapa.c_str());
    }
    logger.cout("Escuchando en el puerto: %s\n", servicio_.c_str());
}

} // namespace servidor
