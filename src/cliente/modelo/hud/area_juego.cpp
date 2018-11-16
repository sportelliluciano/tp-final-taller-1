#include "cliente/modelo/hud/area_juego.h"

#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/camara.h"
#include "cliente/servidor.h"

#define ANCHO_MOV_CAMARA 15
#define ALTO_MOV_CAMARA  15 

/**
 * \brief Delay de la cámara en milisegundos.
 */
#define DELAY_CAMARA_MS 200

namespace cliente {

AreaJuego::AreaJuego(Juego& juego_, Servidor& servidor_, Tostador& tostador_) 
    : juego(juego_), servidor(servidor_), tostador(tostador_), 
      mouse_vender(3932, 3939), mouse_mover_tropa(5010, 5023)
{ 
    mouse_vender.configurar_repeticion(true);
    mouse_mover_tropa.set_centrado(true);
}
    
int AreaJuego::obtener_alto() const {
    return alto;
}

int AreaJuego::obtener_ancho() const {
    return ancho;
}

void AreaJuego::set_tamanio(int ancho_, int alto_) {
    ancho = ancho_;
    alto = alto_;
    camara = Camara(ancho, alto);
}

void AreaJuego::renderizar(Ventana& ventana, int x, int y) {
    ventana.setear_viewport(Rectangulo(x, y, ancho, alto));
    
    if ((mover_camara_x != 0) || (mover_camara_y != 0)) {
        if (ventana.obtener_ms() - delay_camara > DELAY_CAMARA_MS) {
            camara.desplazar_camara(Posicion(32 * mover_camara_x, 
                32 * mover_camara_y));
            delay_camara = ventana.obtener_ms();
        }
    }

    juego.renderizar(ventana, camara);

    if (esta_draggeando) {
        ventana.dibujar_rectangulo(drag_start_x, drag_start_y,
            drag_end_x, drag_end_y, 3);
    }

    if (edificio_a_ubicar) {
        int celda_x, celda_y;
        Terreno& terreno = juego.obtener_terreno();
        terreno.obtener_celda(camara.traducir_a_logica(mouse), celda_x, celda_y);

        for (int cx = 0; cx < edificio_a_ubicar->obtener_ancho_celdas(); cx++) {
            for (int cy = 0; cy < edificio_a_ubicar->obtener_alto_celdas(); cy++) {
                Posicion visual = camara.traducir_a_visual(
                    terreno.obtener_posicion(celda_x + cx, celda_y + cy));
                if (terreno.es_construible(celda_x + cx, celda_y + cy)) {
                    Sprite(0).renderizar(ventana, visual.x, visual.y);
                } else {
                    Sprite(1).renderizar(ventana, visual.x, visual.y);
                }
            }
        }
    }

    if (mouse_en_ventana) {
        if (en_modo_vender) {
            ventana.ocultar_mouse();
            mouse_vender.renderizar(ventana, mouse.x, mouse.y);
        } else if (animar_mover_tropas) {
            ventana.ocultar_mouse();
            mouse_mover_tropa.renderizar(ventana, mouse.x, mouse.y);
            if (mouse_mover_tropa.finalizado())
                animar_mover_tropas = false;
        } else {
            ventana.mostrar_mouse();
        }
    } else {
        ventana.mostrar_mouse();
    }
    
    ventana.reestablecer_viewport();
}

void AreaJuego::set_modo_vender(bool habilitado) {
    en_modo_vender = habilitado;
}

bool AreaJuego::seleccionar_edificio(int x, int y) {
    Edificio* nuevo_seleccionado = 
        juego.obtener_terreno()
            .obtener_edificio_en(camara.traducir_a_logica(Posicion(x, y)));
    
    if (edificio_seleccionado)
        edificio_seleccionado->desmarcar();
    
    if (nuevo_seleccionado)
        nuevo_seleccionado->marcar();
    
    edificio_seleccionado = nuevo_seleccionado;

    return edificio_seleccionado != nullptr;
}

bool AreaJuego::seleccionar_tropas(int x0, int y0, int x1, int y1) {
    std::unordered_set<Tropa*> nueva_seleccion =
        juego.obtener_terreno()
            .obtener_tropas_en(
                Rectangulo(
                    camara.traducir_a_logica(Posicion(x0, y0)), 
                    camara.traducir_a_logica(Posicion(x1, y1))
                )
            );
    
    if (!ctrl_presionado) {
        for (Tropa* tropa : unidades_seleccionadas) {
            tropa->desmarcar();
        }
        for (Tropa* tropa : nueva_seleccion) {
            tropa->marcar();
        }
        unidades_seleccionadas = nueva_seleccion;
    } else {
        for (Tropa* tropa : nueva_seleccion) {
            tropa->marcar();
            unidades_seleccionadas.insert(tropa);
        }   
    }
    
    return !unidades_seleccionadas.empty();
}

bool AreaJuego::mouse_click_izquierdo(int x, int y) {
    if (edificio_a_ubicar) {
        int celda_x, celda_y;
        juego.obtener_terreno().obtener_celda(
                camara.traducir_a_logica(Posicion(x, y)), celda_x, celda_y);
        servidor.ubicar_edificio(edificio_a_ubicar->obtener_clase(),
            celda_x, celda_y);
        tostador.hacer_tostada("Ubicando edificio: " + 
            edificio_a_ubicar->obtener_clase());
        edificio_a_ubicar = nullptr;
    }

    if (en_modo_vender) {
        if (!seleccionar_edificio(x, y)) {
            en_modo_vender = false;
            return false;
        }

        if (ctrl_presionado) {
            tostador.hacer_tostada("Edificio vendido: " + 
                edificio_seleccionado->obtener_clase());
            servidor.vender_edificio(edificio_seleccionado->obtener_id());
            edificio_seleccionado = nullptr;
        } else {
            tostador.hacer_tostada(" ** Mantené apretado CTRL al hacer clic para"
                " vender el edificio ** ");
        }
    }

    seleccionar_tropas(0, 0, 0, 0);
    return false;
}

bool AreaJuego::mouse_click_derecho(int x, int y) {
    Posicion pos_logica = camara.traducir_a_logica(Posicion(x, y));
    int celda_x, celda_y;
    juego.obtener_terreno().obtener_celda(
        pos_logica, celda_x, celda_y);
    
    std::vector<int> ids;
    for (Tropa* tropa : unidades_seleccionadas) {
        ids.push_back(tropa->obtener_id());
    }

    servidor.mover_tropas(ids, pos_logica.x, pos_logica.y);
    tostador.hacer_tostada("Mover tropas");
    
    animar_mover_tropas = true;
    mouse_mover_tropa.reiniciar();
    return false;
}

bool AreaJuego::mouse_inicio_arrastre(int x, int y) {
    drag_start_x = x;
    drag_start_y = y;
    esta_draggeando = true;
    return false;
}

bool AreaJuego::mouse_movimiento(int x, int y) {
    mouse.x = x;
    mouse.y = y;

    if (esta_draggeando) {
        drag_end_x = x;
        drag_end_y = y;
    } else {
        if (x < ANCHO_MOV_CAMARA)
            mover_camara_x = -1;
        else if (x > obtener_ancho() - ANCHO_MOV_CAMARA)
            mover_camara_x = 1;
        else
            mover_camara_x = 0;
        
        if (y < ALTO_MOV_CAMARA)
            mover_camara_y = -1;
        else if (y > obtener_alto() - ALTO_MOV_CAMARA)
            mover_camara_y = 1;
        else
            mover_camara_y = 0;
    }
    
    if (en_modo_vender) {
        seleccionar_edificio(x, y);
    }

    return false;
}

bool AreaJuego::mouse_entra(int, int) {
    mover_camara_x = mover_camara_y = 0;
    mouse_en_ventana = true;
    return false;
}

bool AreaJuego::mouse_sale(int, int) {
    mover_camara_x = mover_camara_y = 0;
    mouse_en_ventana = false;
    esta_draggeando = false;
    return false;
}

bool AreaJuego::mouse_fin_arrastre(int x, int y) {
    drag_end_x = x;
    drag_end_y = y;
    esta_draggeando = false;
    seleccionar_tropas(drag_start_x, drag_start_y,
        drag_end_x, drag_end_y);
    seleccionar_edificio(-1, -1);
    return false;
}

bool AreaJuego::teclado_presionado(tecla_t tecla) {
    switch(tecla) {
        case TECLA_ABAJO:
            mover_camara_y = 1;
            break;
        case TECLA_ARRIBA:
            mover_camara_y = -1;
            break;
        case TECLA_IZQUIERDA:
            mover_camara_x = -1;
            break;
        case TECLA_DERECHA:
            mover_camara_x = 1;
            break;
        case TECLA_CTRL:
            ctrl_presionado = true;
            break;
        default:
            break;
    }
    return false;
}

bool AreaJuego::teclado_suelto(tecla_t tecla) {
    switch(tecla) {
        case TECLA_ABAJO:
        case TECLA_ARRIBA:
            mover_camara_y = 0;
            break;
        case TECLA_IZQUIERDA:
        case TECLA_DERECHA:
            mover_camara_x = 0;
            break;
        case TECLA_CTRL:
            ctrl_presionado = false;
            break;
        default:
            break;
    }
    return false;
}

void AreaJuego::ubicar_edificio(const Edificio* edificio) {
    edificio_a_ubicar = edificio;
}

} // namespace cliente
