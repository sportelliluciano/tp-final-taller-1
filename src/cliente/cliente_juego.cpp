#include "cliente/cliente_juego.h"

#include <QApplication>

#include "cliente/lanzador/lanzador.h"

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/modelo/pantalla_carga.h"
#include "cliente/partida.h"
#include "cliente/servidor.h"
#include "cliente/sonido/sonido.h"
#include "cliente/video/ventana.h"
#include "comun/log.h"
namespace cliente {

ClienteJuego::ClienteJuego() { }

int ClienteJuego::correr(int argc, char *argv[]) {
    if (corriendo)
        throw std::runtime_error("El juego ya estaba corriendo");
    
    corriendo = true;

    if (!ejecutar_lanzador(argc, argv))
        return EXIT_SUCCESS;
    
    if (!ejecutar_juego())
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

bool ClienteJuego::ejecutar_lanzador(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    Lanzador lanzador(partida);
    lanzador.show();

    app.exec();

    return partida.partida_lista();
}

bool ClienteJuego::ejecutar_juego() {
    if (!partida.partida_lista())
        return false;
    
    Ventana ventana(partida.ancho_ventana(), partida.alto_ventana(),
        partida.pantalla_completa(), partida.vsync());
    
    Servidor *servidor = partida.servidor();
    partida.servidor(nullptr);
    // TODO: configurar sonido

    servidor->iniciar_comunicacion_asincronica();

    PantallaCarga pantalla_carga;

    Juego juego("ordos"); //partida.casa());

    while (!juego.inicializacion_completa()) {
        while (servidor->hay_eventos()) {
            Evento *evento = servidor->pop_evento();
            try {
                evento->actualizar(juego);
            } catch (const std::exception& e) {
                log_error("Evento inválido: %s", e.what());
            }
            delete evento;
        }
        pantalla_carga.renderizar(ventana);
        ventana.actualizar();
    }

    // Informar al servidor que el jugador está listo
    servidor->sincronizar_inicio();

    while (!juego.inicio_sincronizado()) {
        while (servidor->hay_eventos()) {
            Evento *evento = servidor->pop_evento();
            try {
                evento->actualizar(juego);
            } catch (const std::exception& e) {
                log_error("Evento inválido: %s", e.what());
            }
            delete evento;
        }
        pantalla_carga.renderizar(ventana);
        ventana.actualizar();
    }

    Controlador controlador(ventana, *servidor, juego);
   
    while (!juego.esta_terminado()) {
        // Renderizar el juego
        controlador.renderizar();
        
        // Mostrar los cambios
        ventana.actualizar();

        // Procesar eventos
        ventana.procesar_eventos(); // Mouse / teclado
        controlador.procesar_entrada(); // Servidor 
        
        // Actualizar el modelo del juego por último para salir
        //  si se detecta que terminó
        juego.actualizar(ventana.obtener_ms());
    }

    servidor->detener();
    delete servidor;
    
    Sonido::obtener_instancia().apagar();
    return true;
}

ClienteJuego::~ClienteJuego() { 
    if (partida.servidor()) {
        partida.servidor()->detener();
        delete partida.servidor();
    }
}

} // namespace cliente
