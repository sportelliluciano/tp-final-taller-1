#include "cliente/modelo/hud/tostada.h"

#include <string>

#include "cliente/video/color.h"
#include "cliente/video/ventana.h"

#define MIN_ANCHO_TOSTADA 350
#define PADDING 5
#define COLOR_FONDO_TOSTADA Color(0x22, 0x22, 0x22, 180)

#define PREFIJO_TOSTADA "tostada-"

namespace cliente {

Tostada::Tostada(const std::string& mensaje, int duracion_)
 : texto(mensaje), duracion(duracion_), terminada(false), fin_ms(0), 
   iniciada(false)
{ 
    id_textura = PREFIJO_TOSTADA + mensaje;
}

bool Tostada::esta_terminada() const {
    return terminada;
}

int Tostada::obtener_alto(Ventana& ventana) const {
    return obtener_textura(ventana).obtener_alto();
}

Textura& Tostada::crear_textura_tostada(Ventana& ventana) const {
    AdministradorTexturas& admin_texturas =
        ventana.obtener_administrador_texturas();
    Rectangulo encuadre(0, 0, ventana.ancho(), ventana.alto());
    
    Textura t_texto = admin_texturas.crear_texto(texto, encuadre, 
        COLOR_BLANCO);
    
    int ancho_tostada = MIN_ANCHO_TOSTADA;
    int ancho_texto = t_texto.obtener_ancho() + 2 * PADDING;

    if (ancho_texto > ancho_tostada)
        ancho_tostada = ancho_texto;
    
    Textura& textura = admin_texturas.crear_textura(id_textura, 
            ancho_tostada, t_texto.obtener_alto());
    
    textura.limpiar(COLOR_FONDO_TOSTADA);
    t_texto.renderizar(PADDING, 0, textura);
    
    return textura;
}

Textura& Tostada::obtener_textura(Ventana& ventana) const {
    AdministradorTexturas& admin_texturas =
        ventana.obtener_administrador_texturas();

    if (admin_texturas.contiene_textura(id_textura))
        return admin_texturas.obtener_textura(id_textura);
    
    return crear_textura_tostada(ventana);
}

void Tostada::renderizar(Ventana& ventana, int x, int y) {
    if (terminada)
        return;
    
    if (!iniciada) {
        iniciada = true;
        fin_ms = ventana.obtener_ms() + duracion;
    }
    
    obtener_textura(ventana).renderizar(x, y);

    if (fin_ms < ventana.obtener_ms()) {
        terminada = true;
    }
}

} // namespace cliente