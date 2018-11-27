# Informe preentrega

Este informe está hecho a partir del commit 777099a7

## Código

### Generales

* **Informe**: El informe (el del trabajo, no esta devolución) tiene que estar divido en 3: *informe técnico*, *informe general del proyecto* y *manual de usuario*.
El informe entregado corresponde más al informe general, faltaría uno con una descripción más técnica, donde explique, por ejemplo, las clases desarrolladas, la relación entre ellas, como interactúan los distintos objetos entre sí, cómo se sincronizan, etc. El manual de usuario es para el usuario que quiera jugar al juego. Debería explicar cómo se instala, cómo se inicia, cómo crear partidas, cómo se juega, etc.

* **Instalador**: falta script de instalación. Utilizar algún mecanismo que me permita instalar el juego en el sistema o en alguna carpeta de preferencia del usuario. Puede ser propio, o puede ser utilizando la directiva `install` de CMake (en este caso se recomienda hacer uso de la variable `CMAKE_INSTALL_PREFIX`).

* **Repositorio**: Limpiar archivos no usados (carpeta draft, mock-sv, data)

### Carpetas compartidas

* Clases/interfaces como `IModelo`, `IJugador`, `Lock` están en la carpeta `conexión` ¿No deberían ir a otra carpeta más general?

### Servidor + modelo

* ¿El modelo es compartido o es parte del servidor?

* Eliminar código muerto en `a_estrella`

* Hay clases sin documentar en la carpeta `modelo`. Si los métodos tienen nombres muy autoexplicativos no es necesario documentarlos, pero las clases sí.

* Explosión va con S

* Hay strings pasados por copia en varias clases de `modelo`

* Limpiar `mati-unit-tests.cpp`, `mock_modelo.h`

### Cliente

* Emprolijar las clases correspondientes al modelo, sobre todo ciertos métodos Renderizar, que son bastante complejos y con mucha lógica.

* Quitar rutas hardcodeadas de la clase `sonido.cpp`. Mover a un header con constantes o a un "archivo de configuración"

* La clase `Log` se encuentra en `video`, pero clases referidas a sprites se encuentran fuera. Revisar cómo están agrupando los archivos fuente.

### Editor

* **El código se encuentra en otro repositorio**

* **No está integrado a la cadena de compilación**

* Los comentarios van en los headers, no en el .cpp (pensar que si se distribuye en forma de biblioteca precompilada, se distribuyen los headers + la biblioteca propiamente dicha)

* Strings pasandose por copia, constantes hardcodeadas.

* Emprolijar la clase Mapa

## UX

### Editor

* Si juego un poco con el tamaño del mapa, puedo agregar spawn de jugadores fuera del terreno.

* El mapa se visualiza mal para grillas menores a 20x20

* Al cargar el mapa no actualiza el valor de los inputs de tamaño

* Se derrocha mucho espacio en blanco en la interfaz, la zona del mapa ocupa lo mismo que los tabs para seleccionar el tipo de terreno. Estos tabs derrochan muchísimo espacio en blanco, queda mejor una distribución, por ejemplo, en una franja de ancho fijo con todos los tipos de terreno y que sea scrolleable. Revisar la distribución de los botones.

* Si el cliente le delegó al editor de mapa la responsabilidad de elegir el sprite adecuado para cada tipo de terreno (por ejemplo, roca2 es para mostrar roca limitando con arena, roca1 es central, etc), debería hacerse de forma automática. El usuario debería seleccionar un sólo tipo de terreno y, automáticamente, se debería reconocer cuál es el subtipo correcto a mostrar. Esto se puede hacer recorriendo la matriz del mapa, de a filas y de a columnas, y detectando casos como [arena, roca, arena], [arena, roca, roca], [roca, roca, arena] y [roca, roca, roca], y utilizar las texturas correspondientes para estos casos borde.

* Agregar un menú, que tenga la opción de guardar y cargar mapas.

### Servidor

* Agregar un `-h, --help`

* La aplicación inica y no hay feedback, no indica que se queda esperando 'q' para salir ni en qué puerto está escuchando.

* En el enunciado se pide un archivo de configuración:  El servidor deberá recibir por parámetro la ruta a un archivo de configuración que contendrá puerto dónde escuchar, rutas los archivos con los mapas (puede ser una ruta con el directorio de los mapas), parámetros del juego (daño, vida, etc). Ya que se están usando varios archivos json, el archivo de configuración podría tener la ruta de estos mismos.

* Revisar lógica de energía y especias. Los edificios parecen estar pagándose con energía en vez de especias, las trampas de aire no están dando energía. ¿El límite de especias según los edificios construidos está implementado?

* Agregar lógica de unidades que se pueden construir según los edificios disponibles

* Condición de victoria/derrota.

* No iniciar el juego si no hay jugadores suficientes.

* Si el jugador se desconecta, el juego sigue abierto. Eso, sumado a que me puedo unir a partidas en ejecución resulta en que si me vuelvo a unir, la partida explota.

### Cliente

* **No pude iniciar 2 clientes en simultaneo**

#### Pantalla de conexión

* Dividir la pantalla en pasos. Mostrar primero el grupo de la izquierda (los de conectar y crear sala), una vez conectado, esconder este grupo y mostrar los grupos de la derecha (elegir nombre, ver jugadores).

* Hacer un diseño fluido ("responsive")

#### Juego

* Falta la vista del minimapa. darle un fondo al area de botones

* [Corregido] ~~Limitar el desplazamiento de la cámara para que no se desplace infinitamente.~~

* No pude probar los sonidos pedidos por el enunciado
