#include "cliente/modelo/hud/area_juego.h"

#include "cliente/modelo/hud/tostador.h"
#include "cliente/modelo/juego.h"
#include "cliente/video/camara.h"
#include "cliente/modelo/sprite.h"
#include "cliente/servidor.h"

#define ANCHO_MOV_CAMARA 15
#define ALTO_MOV_CAMARA  15 

#define VELOCIDAD_CAMARA 4

namespace cliente {

AreaJuego::AreaJuego(Juego& juego_, Servidor& servidor_, Tostador& tostador_) 
    : juego(juego_), ejercito(juego.obtener_ejercito()), 
      infraestructura(juego.obtener_infraestructura()), 
      servidor(servidor_), tostador(tostador_), 
      mouse_vender(3932, 3939), mouse_mover_tropa(5010, 5023)
{ 
    mouse_vender.configurar_repeticion(true);
    mouse_mover_tropa.set_centrado(true);
    mouse_atacar = SpriteAnimado({Sprite("./assets/nuevos/atacar.png")}, 1);
    mouse_atacar.configurar_repeticion(true);
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

void AreaJuego::renderizar(Ventana& ventana, const Posicion& punto) {
    ventana.setear_viewport(Rectangulo(punto.x, punto.y, ancho, alto));
    
    if ((mover_camara_x != 0) || (mover_camara_y != 0)) {
        camara.desplazar_camara(Posicion(VELOCIDAD_CAMARA * mover_camara_x, 
            VELOCIDAD_CAMARA * mover_camara_y));
    }

    juego.renderizar(ventana, camara);

    if (esta_draggeando) {
        ventana.dibujar_rectangulo(drag_start.x, drag_start.y,
            drag_end.x, drag_end.y, 3);
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
        if (sprite_mouse) {
            ventana.ocultar_mouse();
            sprite_mouse->renderizar(ventana, mouse.x, mouse.y);
            if (sprite_mouse->finalizado())
                sprite_mouse = nullptr;
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
    if (habilitado)
        sprite_mouse = &mouse_vender;
    else
        sprite_mouse = nullptr;
}

bool AreaJuego::seleccionar_edificio(const Posicion& punto) {
    Edificio* nuevo_seleccionado = 
        juego.obtener_terreno()
            .obtener_edificio_en(camara.traducir_a_logica(punto));
    
    deseleccionar_edificio();
    
    if (nuevo_seleccionado)
        nuevo_seleccionado->marcar();
    
    edificio_seleccionado = nuevo_seleccionado;

    return edificio_seleccionado != nullptr;
}

void AreaJuego::deseleccionar_edificio() {
    if (edificio_seleccionado)
        edificio_seleccionado->desmarcar();
    edificio_seleccionado = nullptr;
}

bool AreaJuego::seleccionar_tropas(int x0, int y0, int x1, int y1) {
    std::unordered_set<Tropa*> nueva_seleccion =
        ejercito.obtener_tropas_propias_en(
                Rectangulo(
                    camara.traducir_a_logica(Posicion(x0, y0)), 
                    camara.traducir_a_logica(Posicion(x1, y1))
                )
            );
    
    if (!ctrl_presionado) {
        deseleccionar_tropas();
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

void AreaJuego::deseleccionar_tropas() {
    for (Tropa* tropa : unidades_seleccionadas) {
        tropa->desmarcar();
    }
    unidades_seleccionadas.clear();
}

bool AreaJuego::mouse_click_izquierdo(const Posicion& punto) {
    if (edificio_a_ubicar) {
        int celda_x, celda_y;
        juego.obtener_terreno().obtener_celda(
                camara.traducir_a_logica(punto), celda_x, celda_y);
        servidor.ubicar_edificio(edificio_a_ubicar->obtener_clase(),
            celda_x, celda_y);
        tostador.hacer_tostada("Ubicando edificio: " + 
            edificio_a_ubicar->obtener_clase());
        edificio_a_ubicar = nullptr;
    }

    if (en_modo_vender) {
        if (!seleccionar_edificio(punto)) {
            set_modo_vender(false);
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

    deseleccionar_tropas();
    return false;
}

bool AreaJuego::mouse_click_derecho(const Posicion& punto) {
    if (unidades_seleccionadas.empty())
        return false;
    
    Posicion pos_logica = camara.traducir_a_logica(punto);
    
    std::vector<int> ids;
    for (Tropa* tropa : unidades_seleccionadas) {
        ids.push_back(tropa->obtener_id());
    }

    if (ejercito.hay_tropas_enemigas_en(camara.traducir_a_logica(mouse))) {
        Tropa* enemigo = 
            ejercito.obtener_tropa_enemiga_en(camara.traducir_a_logica(mouse));
        servidor.atacar_tropa(ids, enemigo->obtener_id());
        tostador.hacer_tostada("Atacar tropas");    
    } else {
        servidor.mover_tropas(ids, pos_logica.x, pos_logica.y);
        tostador.hacer_tostada("Mover tropas");
        mouse_mover_tropa.reiniciar();
        sprite_mouse = &mouse_mover_tropa;
    }
    
    return false;
}

bool AreaJuego::mouse_inicio_arrastre(const Posicion& punto) {
    drag_start = punto;
    esta_draggeando = true;
    return false;
}

bool AreaJuego::mouse_movimiento(const Posicion& punto) {
    mouse = punto;

    if (esta_draggeando) {
        drag_end = punto;
        return false;
    }
    
    if (punto.x < ANCHO_MOV_CAMARA)
        mover_camara_x = -1;
    else if (punto.x > obtener_ancho() - ANCHO_MOV_CAMARA)
        mover_camara_x = 1;
    else
        mover_camara_x = 0;
    
    if (punto.y < ALTO_MOV_CAMARA)
        mover_camara_y = -1;
    else if (punto.y > obtener_alto() - ALTO_MOV_CAMARA)
        mover_camara_y = 1;
    else
        mover_camara_y = 0;
    
    if (en_modo_vender) {
        seleccionar_edificio(punto);
    } else if (!unidades_seleccionadas.empty() &&
        ejercito.hay_tropas_enemigas_en(camara.traducir_a_logica(mouse))) 
    {
        if (sprite_mouse == nullptr)
            sprite_mouse = &mouse_atacar;
    } else if (sprite_mouse == &mouse_atacar) {
        sprite_mouse = nullptr;
    }

    return false;
}

bool AreaJuego::mouse_entra(const Posicion&) {
    mover_camara_x = mover_camara_y = 0;
    mouse_en_ventana = true;
    return false;
}

bool AreaJuego::mouse_sale(const Posicion&) {
    mover_camara_x = mover_camara_y = 0;
    mouse_en_ventana = false;
    esta_draggeando = false;
    return false;
}

bool AreaJuego::mouse_fin_arrastre(const Posicion& punto) {
    drag_end = punto;
    esta_draggeando = false;
    seleccionar_tropas(drag_start.x, drag_start.y, drag_end.x, drag_end.y);
    deseleccionar_edificio();
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

void AreaJuego::centrar_camara() {
    Posicion centro = juego.obtener_centro();
    Posicion centro_camara(
        centro.x - (camara.obtener_vista().ancho() / 2),
        centro.y - (camara.obtener_vista().alto() / 2)
    );
    camara.mover_camara(centro_camara);
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
        case TECLA_F2:
            centrar_camara();
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
