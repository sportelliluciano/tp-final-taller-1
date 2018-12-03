#include "cliente/cliente_juego.h"

#include <QApplication>

#include "cliente/lanzador/ventana_principal.h"

#include "cliente/modelo/controlador.h"
#include "cliente/modelo/juego.h"
#include "cliente/modelo/pantalla_carga.h"
#include "cliente/red/partida.h"
#include "cliente/red/servidor.h"
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
    
    VentanaPrincipal lanzador(partida);
    lanzador.show();

    app.exec();

    return partida.partida_lista();
}

static bool procesar_eventos(Juego& juego, Servidor& servidor) {
    while (servidor.hay_eventos()) {
        Evento *evento = servidor.pop_evento();
        try {
            evento->actualizar(juego);
        } catch (const std::exception& e) {
            return false;
        }
        delete evento;
    }

    // Verificar si la conexión sigue abierta
    return servidor.esta_conectado();
}

static bool realizar_ciclo_carga(Ventana& ventana, PantallaCarga& pantalla, 
    Juego& juego, Servidor& servidor)
{
    if (!procesar_eventos(juego,  servidor))
        return false;
    pantalla.renderizar(ventana);
    ventana.actualizar();
    if (!ventana.procesar_eventos())
        return false;
    return true;
}

static bool cargar_juego(Juego& juego, Servidor& servidor, Ventana& ventana) {
    servidor.iniciar_comunicacion_asincronica();

    // Esperar datos de inicialización
    PantallaCarga pantalla_carga;
    while (!juego.inicializacion_completa()) {
        if (!realizar_ciclo_carga(ventana, pantalla_carga, juego, servidor))
            return false;
    }

    // Informar al servidor que el jugador está listo
    servidor.sincronizar_inicio();

    // Esperar sincronización con otros clientes
    while (!juego.inicio_sincronizado()) {
        if (!realizar_ciclo_carga(ventana, pantalla_carga, juego, servidor))
            return false;
    }

    return true;
}

bool ClienteJuego::ejecutar_juego() {
    if (!partida.partida_lista())
        return false;
    
    Sonido& sonido = Sonido::obtener_instancia();
    Ventana ventana(partida.ancho_ventana(), partida.alto_ventana(),
        partida.pantalla_completa(), partida.vsync());
    
    Servidor *servidor = partida.servidor();
    partida.servidor(nullptr);

    Juego juego(partida.casa());

    if (!cargar_juego(juego, *servidor, ventana))
        return false;

    Controlador controlador(ventana, *servidor, juego);
    sonido.habilitar_sonidos(partida.sonido());
    if (partida.musica())
        sonido.iniciar_musica_fondo();
    
    while (!juego.esta_terminado()) {
        // Renderizar el juego
        controlador.renderizar();
        
        // Mostrar los cambios
        ventana.actualizar();

        // Procesar eventos
        ventana.procesar_eventos(); // Mouse / teclado
        controlador.procesar_entrada(); // Servidor 

        // Actualizar sonidos
        sonido.actualizar();
        
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
