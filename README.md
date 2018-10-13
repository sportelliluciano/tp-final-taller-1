# Trabajo práctico final de Taller 1: Dune 2000

#### Lineamientos sobre el código:

- Nombres de clases, variables y atributos en español.
- PascalCase para nombres de clases
- snake_case para nombres de métodos / atributos / variables
- Documentación en estilo Doxygen:
```cpp
/**
 * \brief Descripción corta de la función.
 * 
 * Descripción detallada de la función.
 */
```
- Nombres de archivos nombre_de_clase_en_snake_case.h/.cpp
- `#include` con rutas relativas a la carpeta "src" [Tomado de la guia de Google]. Por ejemplo para `tp-final/src/cliente/ventana.h` quedaría `#include "cliente/ventana.h"` (independientemente de si el archivo está dentro de la carpeta cliente o no). Hasta donde entiendo esto es más que nada para que quede claro de donde pertenece algun archivo; idealmente sólo sería `cliente/` o `comun/` pero mientras esté en desarrollo capaz existan inclusiones desde el servidor o el editor (hasta que finalmente quede claro qué cosas son comunes).

#### Para compilar:
```bash
~/tp-final-taller-1 $ mkdir build
~/tp-final-taller-1 $ cd build
~/tp-final-taller-1/build $ cmake ..
~/tp-final-taller-1/build $ make
```
Con eso se deben generar los siguientes ejecutables:
- Cliente del juego: `build/cliente`
- Servidor del juego: `build/servidor`
- Editor de mapas del juego: `build/editor`
- Pruebas unitarias del servidor: `build/test/servidor-unit-tests`
- Pruebas unitarias del cliente: `build/test/cliente-unit-tests`
- Pruebas unitarias del editor: `build/test/editor-unit-tests`

El cliente requiere `libsdl2-dev` y `libsdl2-image-dev` para que compile.

#### Sobre el versionado:
Según escuché [Lucho] el día que presentaron el enunciado (por el final cuando ya se había ido medio curso), ellos esperan que el historial del repo sea lo más descriptivo posible, al punto de intentar mantener un commit por archivo o grupos pequeños de archivos (no hacer un gran commit alterando medio servidor entero de una). Idealmente habría un commit por archivo modificado (muerte a `git add .`).

En resumido:
- Commits chicos de un archivo o un par de archivos con mensajes específicos


#### Sobre las pruebas unitarias
Hay un micro framework (que escribí para el ejercicio 3) _ad-hoc_ de pruebas unitarias armado específicamente para testear cada clase que así lo requiera. Para crear las pruebas unitarias alcanza con crear un archivo en la carpeta `test/cliente`, `test/servidor` o `test/editor` según corresponda que lleve de nombre `test-{nombre del archivo con la clase a testear}.h`. Por ejemplo si se quiere testear la clase en `cliente/ventana.h`, se debe crear un archivo `test/cliente/test-ventana.h` (notar que, por omisión `cliente/ventana.h` hace referencia a `src/cliente/ventana.h`, esto sucede en el código de la misma forma).

Hay un ejemplo "totalmente" documentado sobre como crear las pruebas unitarias para una clase
en `test/cliente/test-a.h`. Esa prueba funciona y se puede correr y ver los resultados.

La idea de este microframework es justamente que sea lo mínimo e  indispensable para poder armar las pruebas unitarias que sean necesarias sin tener que luchar demasiado con el código. Agregar pruebas para una clase implican únicamente copiar el archivo `test/test-plantilla.h` a donde  corresponda, cambiar algunos nombres, escribir los tests y compilar.

En particular considero que no es necesario realizar fuertes pruebas unitarias sobre todo el trabajo, sino únicamente en las clases que así lo requieran; probablemente sea un poco complicado probar el modelo sin tener cliente-servidor funcionando, y las pruebas unitarias podrían ayudar.

#### Sobre CMake
_Esto es maravilloso._

Archivos CMake que están tirados por ahí:
- Raíz: `./CMakeLists.txt`
Genera cliente, servidor, editor
- Cliente: `src/cliente/CMakeLists.txt`
Genera libcliente [acá deberían agregarse librerías necesarias por el cliente]
- Servidor: `src/servidor/CMakeLists.txt`
Genera libservidor [acá deberían agregarse librerías necesarias por el servidor]
- Editor: `src/editor/CMakeLists.txt`
Genera libeditor [acá deberían agregarse librerías necesarias por el editor]
- Pegamento para tener una carpeta src: `src/CMakeLists.txt`
No hace gran cosa
- Pruebas unitarias: `test/CMakeLists.txt`
Genera cliente-unit-test, servidor-unit-test, editor-unit-test

