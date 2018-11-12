#ifndef _ADMINISTRADOR_TEXTURAS_H_
#define _ADMINISTRADOR_TEXTURAS_H_

#include <map>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "cliente/video/textura.h"

namespace cliente {

class AdministradorTexturas {
public:
    /**
     * \brief Crea un nuevo administrador de texturas para un determinado 
     *        renderer.
     */
    AdministradorTexturas(SDL_Renderer *renderer_);

    /**
     * \brief Carga una imagen desde un archivo a una textura.
     * 
     * Almacena las texturas en una caché de modo que al volver a usarlas 
     * no haya que levantarlas desde el disco.
     */
    const Textura& cargar_imagen(const std::string& img);

    /**
     * \brief Devuelve true si el almacén contiene la textura indicada.
     */
    bool contiene_textura(const std::string& id);

    /**
     * \brief Obtiene una textura del almacén de texturas creadas.
     * 
     * Al crear una nueva textura la misma se almacena en el almacén de texturas
     * de la ventana. Este método permite obtener la textura.
     */
    Textura& obtener_textura(const std::string& id);

    /**
     * \brief Elimina una textura del almacén. Devuelve la textura eliminada.
     */
    Textura eliminar_textura(const std::string& id);

    /**
     * \brief Crea una nueva textura vacía.
     * 
     * Crea una nueva textura vacía y la guarda en el almacén de texturas con el
     * nombre indicado. Si el nombre ya existe lanza una excepción.
     * Devuelve la textura creada. Se puede obtener la misma llamando al método
     * obtener_textura pasándole como parámetro el nombre de la misma.
     */
    Textura& crear_textura(const std::string& nombre, int w, int h);

    /**
     * \brief Crea una nueva textura con el texto indicado y la almacena
     *        junto con las texturas creadas con el nombre "texto-" + texto.
     */
    Textura& crear_texto(const std::string& texto);

    /**
     * \brief Crea una nueva textura con el texto indicado sin almacenarla.
     */
    Textura crear_texto_nc(const std::string& texto);

    /**
     * \brief Destructor.
     * 
     * Libera todas las texturas almacenadas en caché.
     */
    ~AdministradorTexturas();

private:
    SDL_Renderer *renderer;
    TTF_Font* fuente;
    std::unordered_map<std::string, Textura> texturas;
    std::map<std::string, Textura> texturas_creadas;
};

} // namespace cliente

#endif // _ADMINISTRADOR_TEXTURAS_H_
