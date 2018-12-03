#ifndef _PARTIDA_H_
#define _PARTIDA_H_

#include <string>

#include "cliente/red/servidor.h"

namespace cliente {

/**
 * \brief Administrador de la partida del juego.
 * 
 * Permite configurar distintos parámetros de la partida antes de que inicie
 * el juego
 */
class Partida {
public:
    Partida();

    /**
     * \brief Activa o desactiva la pantalla completa del juego.
     * 
     * La resolución del cliente en pantalla completa será la nativa del 
     * sistema.
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void pantalla_completa(bool activar);
    bool pantalla_completa() const;

    /**
     * \brief Configura el tamaño de la ventana.
     */
    void tamanio_ventana(int ancho, int alto);

    /**
     * \brief Devuelve el ancho/alto de la ventana.
     * 
     * Este resultado no tiene ningún significado si el juego se ejecuta a
     * pantalla completa.
     */
    int ancho_ventana() const;
    int alto_ventana() const;

    /**
     * \brief Activa o desactiva el sincronizmo vertical.
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void vsync(bool activar);
    bool vsync() const;

    /**
     * \brief Activa o desactiva la música durante el juego.
     * 
     * Esta configuración se puede cambiar durante el juego.
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void musica(bool activar);
    bool musica() const;

    /**
     * \brief Activa o desactiva los efectos de sonido durante el juego.
     * 
     * Esta configuración se puede cambiar durante el juego.
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void sonido(bool activar);
    bool sonido() const;

    /**
     * \brief Configura el nombre de la sala donde se va a jugar.
     * 
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void nombre_sala(const std::string& nombre);
    const std::string& nombre_sala() const;

    /**
     * \brief Configura el nombre del mapa a jugar.
     * 
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     */
    void mapa(const std::string& nombre_mapa);
    const std::string& mapa() const;

    /**
     * \brief Configura el servidor a usar durante la partida.
     * 
     * Llamarlo sin parámetros obtiene el servidor configurado actualmente.
     * 
     * NOTA (1): El servidor debe crearse mediante new, ya que será absorvido 
     * por la partida al iniciarse y luego destruido por la misma al finalizar 
     * utlizando delete.
     * 
     * NOTA (2): El servidor pasado **debe** estar previamente iniciado.
     */
    void servidor(Servidor* nuevo_servidor);
    Servidor* servidor();

    /**
     * \brief Configura la casa del jugador actual.
     */
    void casa(const std::string& casa);
    const std::string& casa() const;

    /**
     * \brief Indica que la partida está lista para arrancar.
     * 
     * Llamarlo sin parámetros obtiene el valor actual de la configuración.
     * NOTA: Si se setea partida_lista en true pero no se configura un servidor,
     * entonces el valor obtenido será falso. Para que partida_lista devuelva
     * true tiene que haberse seteado en este estado **y además** haberse 
     * configurado un servidor. No importa el orden en que se realicen las
     * acciones (primero setear el servidor y luego la partida o viceversa)
     */
    void partida_lista(bool esta_lista);
    bool partida_lista() const;

private:
    bool _pantalla_completa = false, 
         _vsync = true, 
         _musica = false, 
         _sonido = true,
         _esta_lista = false;
    
    std::string _nombre_sala, _mapa, _casa;

    int _ancho_ventana = 1024, _alto_ventana = 600;

    Servidor* _servidor = nullptr;
};

} // namespace cliente

#endif // _PARTIDA_H_

