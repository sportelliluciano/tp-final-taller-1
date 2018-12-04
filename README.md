# Trabajo práctico final de Taller 1: Dune 2000

Link al informe técnico: https://www.overleaf.com/read/rgzkdzcxtbbx

Link al informe general: https://es.overleaf.com/read/vjspvynrnrct

Link al manual de usuario: https://www.overleaf.com/read/ymgsrcqhpgjg

### Para compilar el cliente
#### Bibliotecas requeridas:
- `libsdl2-dev`
- `libsdl2-image-dev`
- `libsdl2-mixer-dev`
- `libsdl2-ttf-dev`
- `qt5-default`

#### Utilidades requeridas
- `python3`
- `7z`

#### Archivos requeridos
Para compilar el proyecto se debe crear una carpeta (comunmente denominada 
`build`) y se deben copiar dentro de la misma los archivos `sprites.7z` y
`terrain.7z` provistos por la cátedra.
Todos los archivos extra requeridos se encuentran en la carpeta `data` y serán
copiados al lugar correspondiente por el instalador del juego.

Asumiendo que se compilará dentro de la carpeta `build`, la estructura debe
quedar de la siguiente manera:

Observar que la carpeta `build` esta ignorada en el `.gitignore`.

```
build/
├── sprites.7z
└── terrain.7z
```

El script de instalación se ocupará de descomprimir los mismos y copiarlos en
la ubicación adecuada.

#### Compilación e instalación
Una vez creada la carpeta `build` y copiados los archivos `terrain.7z` y
`sprites.7z` se debe proceder de la siguiente forma para compilar el proyecto.
```bash
~/tp-final-taller-1 $ cd build
~/tp-final-taller-1/build $ cmake -DCMAKE_INSTALL_PREFIX=./instalacion ..
~/tp-final-taller-1/build $ make
~/tp-final-taller-1/build $ make install
```

El valor de `CMAKE_INSTALL_PREFIX` permite determinar donde se instalará el
juego. Por defecto este directorio es `/usr/local`.

Se admiten las siguientes opciones de compilación:
- `CL_USAR_ASAN`: Compilar el cliente con Address Sanitizer
- `DEPURAR_RED`: Mostrar los mensajes de red enviados tanto el cliente como
en el servidor.
- `SV_DEPURAR_TIEMPOS`: Mostrar mensajes de depuración de tiempos en el servidor.

Por defecto las opciones están desactivadas.

Para activar alguna de las opciones, agregarlas en el comando cmake en la forma
`-Dopcion=ON|OFF`, por ejemplo:

```bash
cmake -DSV_DEPURAR_TIEMPOS=ON -DCMAKE_INSTALL_PREFIX=./instalacion ..
```

Opcionalmente se puede agregar a la variable de entorno `PATH` la ruta
`<CMAKE_INSTALL_PREFIX>/bin` para poder ejecutar el mismo directamente.


#### Ejecución del juego
El juego se compone de dos binarios, el cliente y el servidor. El cliente
se ejecuta como `dune-remake` y el servidor como `dune-remake-servidor`.


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

El editor requiere 'qt5-default' para que compile.

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

