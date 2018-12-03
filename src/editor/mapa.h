#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <map>
#include <vector>
#include "editor/label_mapa.h"
#include "editor/observador.h"
#include "editor/observador_mapa.h"
#include "editor/generador_sprites.h"

class Mapa : public ObservadorMapa {
    public:
        /**
         * \brief 1er Constructor de Mapa.
         * 
         * Constructor de Mapa que es utilizado cuando se crea un mapa desde 0. 
         * Recibe como parametros el tamaño (filas y columnas) y la cantidad de jugadores.
         */
        Mapa(int filas, int columnas, QWidget* parent = 0);
        
        /**
         * \brief 2do Constructor de Mapa.
         * 
         * Constructor de Mapa que es utilizado cuando se carga un mapa. El tamaño y la
         * cantidad de jugadores se define en Mapa::parsear_json.
         */
        explicit Mapa(QWidget* parent = 0);
        
        int get_cant_filas();
        int get_cant_columnas();
        
        /**
         * \brief Parseo del json Mapa.
         * 
         * Parseo del json con la informacion de un mapa creado y almacenado anteriormente.
         * Precondicion -> haber construido Mapa con el 2do Constructor de Mapa.
         * 
         */
        void parsear_json(std::string& filename_json);

        /**
         * \brief Inicializacion Mapa.
         * 
         * Precondicion -> haber construido Mapa con el 1er Constructor de Mapa.
         */
        void inicializar_mapa();

        /**
         * \brief Agrega jugador al Mapa.
         * 
         * Agrega jugador al mapa jugadores y actualiza la imagen del LabelMapa
         * correspondiente.
         */
        void agregar_jugador(std::string& id_label, Sprite nuevo_sprite);

        /**
         * \brief Agrego observador del Mapa.
         * 
         * Agrego un observador del Mapa (Editor).
         */
        void agregar_observador(Observador* observer);
        
        /**
         * \brief Getter tipo de LabelMapa.
         * 
         * Devuelvo el tipo del LabelMapa cuyo id es el especificado en el parámetro 
         * (delego en LabelMapa). 
         * Precondicion -> el id_label_mapa es correcto.
         */
        int get_tipo_by_id(std::string& id_label_mapa);

        /**
         * \brief Validez agregar jugador.
         * 
         * Verifico que no haya un jugador en el LabelMapa cuyo id recibo por parametro.
         */
        bool es_valido_agregar_jugador(std::string& id_label_mapa, 
            int cantidad_jugadores);

        /**
         * \brief Genero json de Mapa.
         * 
         * Genero json con la informacion de Mapa. Recibo el nombre del archivo que tengo 
         * que generar por parametro (filepath incluido).
         */        
        void generar_json(std::string& nombre_archivo);


        /**
         * \brief Actualizar data.
         * 
         * Actualizo la data de un LabelMapa cuyo id recibo por parametro (delego 
         * en LabelMapa). 
         * Precondicion -> Ya fue contemplado el caso en que tipo_label_mapa y nuevo_tipo
         * son Jugador. 
         */
        void actualizar_data(std::string& id_label, Sprite nuevo);

        /**
         * \brief Metodo virtual de la interfaz ObservadorMapa.
         * 
         * Metodo virtual que es llamado por LabelMapa cuando este es clickeado.
         * Avisa al observador cual LabelMapa fue clickeado.
         */
        virtual void label_mapa_clickeado(std::string& id_label_mapa);

        /**
         * \brief Getter cantidad de jugadores agregados.
         * 
         * Devuelvo la cantidad de jugadores que fueron agregados al mapa hasta ese
         * momento.
         */
        int get_cantidad_jugadores_agregados();

        /**
         * \brief Cambia tamaño del Mapa.
         * 
         * Recibe el nuevo tamaño del Mapa por parámetro. Delega la implementacion
         * en algunos metodos privados segun si se achica o agranda el mapa.
         */
        void cambiar_tamanio(int nueva_cant_filas, int nueva_cant_columnas);

        /**
         * \brief Limpia Mapa.
         * 
         * Libero los LabelMapa del heap (fueron tomados los recursos en 
         * Mapa::inicializar_mapa o en Mapa::parsear_json).
         */
        void limpiar_mapa();
        
        /**
         * \brief Destructor de Mapa.
         * 
         * Libero los LabelMapa del heap (fueron tomados los recursos en 
         * Mapa::inicializar_mapa o en Mapa::parsear_json).
         */
        ~Mapa();

    private:
        int filas;
        int columnas;
        QWidget* parent;
        Observador* observador;
        std::map<std::string, LabelMapa*> mapa;
        std::map<std::string, bool> jugadores;
        
        void agrandar_mapa(int nueva_cant_filas, int nueva_cant_columnas);
        void achicar_mapa(int nueva_cant_filas, int nueva_cant_columnas);
        void sacar_columnas_agregar_filas(int nueva_cant_filas, 
            int nueva_cant_columnas);
        void sacar_filas_agregar_columnas(int nueva_cant_filas, 
            int nueva_cant_columnas);
};

#endif // MAPA_H