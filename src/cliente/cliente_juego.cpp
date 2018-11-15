#include "cliente/cliente_juego.h"

#include <QApplication>

#include "cliente/lanzador/lanzador.h"

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/partida.h"
#include "cliente/servidor.h"
#include "cliente/sonido/sonido.h"
#include "cliente/video/ventana.h"

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

    // TODO: configurar sonido

    Juego juego;
    
    Servidor *servidor = partida.servidor();
    partida.servidor(nullptr);
    
    Controlador controlador(ventana, *servidor, juego);

    while (!juego.esta_terminado()) {
        // Procesar eventos
        ventana.procesar_eventos(); // Cerrar ventana
        controlador.procesar_entrada(); // Servidor / Mouse / teclado
        
        // Actualizar el modelo del juego
        juego.actualizar(ventana.obtener_ms());
        
        // Renderizar el juego
        controlador.renderizar();

        // Mostrar los cambios
        ventana.actualizar();
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
