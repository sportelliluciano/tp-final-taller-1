#include "cliente/modelo/hud/tostada.h"

#include <string>

#include "cliente/video/color.h"
#include "cliente/video/ventana.h"

#define ALTO_TOSTADA 20

#define COLOR_FONDO_TOSTADA Color(0x22, 0x22, 0x22, 180)

namespace cliente {

Tostada::Tostada(const std::string& mensaje, int duracion_)
 : texto(mensaje), duracion(duracion_), terminada(false), fin_ms(0), 
   iniciada(false)
{ }

bool Tostada::esta_terminada() const {
    return terminada;
}

int Tostada::obtener_alto() const {
    return ALTO_TOSTADA;
}

void Tostada::renderizar(Ventana& ventana, int x, int y) {
    if (terminada)
        return;
    
    if (!iniciada) {
        iniciada = true;
        fin_ms = ventana.obtener_ms() + duracion;
    }
    
    AdministradorTexturas& admin_texturas = ventana
        .obtener_administrador_texturas();


    if (!admin_texturas.contiene_textura("tostada-" + texto)) {
        Textura& textura = admin_texturas.crear_textura("tostada-" + texto, 
            texto.length() * 10, ALTO_TOSTADA);
        
        textura.limpiar(COLOR_FONDO_TOSTADA);
        
        admin_texturas.crear_texto_nc(texto).renderizar(0, 0, textura);
    }
        

    if (fin_ms < ventana.obtener_ms()) {
        terminada = true;
        admin_texturas.eliminar_textura("tostada-" + texto);
        return;
    }

    ventana
        .obtener_administrador_texturas()
        .obtener_textura("tostada-" + texto)
        .renderizar(x, y);    
}

} // namespace cliente