## Archivos de configuración del servidor

### Archivo de configuración general
El servidor carga inicialmente el archivo de configuración llamado `config.json`
que adminte los siguientes parámetros:

- **Puerto**  
*Valor por defecto:* `"puerto": "9432"`  
Este es el puerto en el que escuchará el servidor si no se especifica el mismo
entre los argumentos de línea de comandos. Observar que el valor es un string
y no un número, y puede ser el nombre de un servicio conocido (ej: http).
- **Ruta al archivo de edificios**  
*Valor por defecto:* `"ruta_edificios": "./edificios.json"`  
El archivo de edificios contiene la información relativa a los edificios que
se pueden construir en el juego, sus imágenes y metadatos.
- **Ruta al archivo de ejércitos**  
*Valor por defecto:* `"ruta_ejercitos": "./ejercitos.json"`  
Al igual que el archivo de edificios, el archivo de ejércitos contiene la 
información sobre las tropas, su vida, daño, ataques especiales y demás.
- **Mapas disponibles**  
*Valor por defecto:* `"mapas": [ ]`  
Se debe indicar la ruta a cada uno de los mapas que se pueden jugar en este
servidor. Notar que este parámetro es obligatorio ya que sin el no se podrá
iniciar el servidor. El valor del mismo debe ser una lista de *strings* donde
cada una de ellas será la ruta a un archivo de mapas en formato JSON.

Ejemplo de archivo `config.json`:

```json
{
    "puerto": "9432",
    "ruta_edificios": "./edificios.json",
    "ruta_ejercitos": "./ejercitos.json",
    "mapas": [
        "./mapa-1.json",
        "./mapa-2.json"
    ]
}
```

### Archivo de edificios
El archivo de edificios es el que contiene toda la información requerida por
el servidor y los clientes sobre los edificios. 

El mismo debe estar en formato JSON y contener, **únicamente** un arreglo de
objetos. Cada objeto define un nuevo tipo de edificio y debe tener los 
siguientes parámetros:

- **ID**  
*Valor por defecto:* `"id": "defecto"`  
Este es el identificador del *tipo* de edificio. Debe ser único por cada 
clase de edificio distinto.
- **Nombre**  
*Valor por defecto:* `"nombre": "-"`  
El nombre *legible por humanos* del edificio, para mostrar en la interfaz gráfica.
- **Descripción**  
*Valor por defecto:* `"descripcion": "-"`  
Descripción del edificio para mostrar en la interfaz gráfica. No debe superar los 100 caracteres.
- **Metadata**  
*Valor por defecto:* `"metadata": { }`  
Metadatos del edificio, este campo sólo se utiliza para mostrar información extra al usuario (no afecta al juego). El mismo debe ser un objeto JSON donde la clave es un nombre de atributo y el valor debe ser el valor del mismo. Un ejemplo de esto sería lo siguiente: `{"Energía req.": "500"}`.
- **Energía**  
*Valor por defecto:* `"energia": 0`  
Energía requerida por el edificio.
- **Costo**  
*Valor por defecto:* `"costo": 0`  
Dinero requerido para construir el edificio.
- **Dimensiones**  
*Valor por defecto:* `"dimensiones": [1, 1]`  
Dimensiones, en celdas, del edificio.
- **Tiempo de construcción**  
*Valor por defecto:* `"tiempo_construccion": 1`  
Tiempo, en segundos, requeridos para construir el edificio a velocidad normal.
- **Puntos de estructura**  
*Valor por defecto:* `"puntos_estructura": 1000`  
Puntos de estructura (vida máxima) del edificio.
- **Capacidad de almacenamiento de especia**  
*Valor por defecto:* `"capacidad_especia": 0`  
Si el edificio puede almacenar especia, debe indicarse la cantidad en este campo.
- **Sprite base, construido y destruido**  
*Valor por defecto:* `"sprite_base": {"id": -1, "x": 0, "y": 0}`  
*Valor por defecto:* `"sprite_construido": {"id": -1, "x": 0, "y": 0}`  
*Valor por defecto:* `"sprite_destruido": {"id": -1, "x": 0, "y": 0}`  
Estos sprites definen cómo se dibujará el edificio en la pantalla de los clientes. El sprite *construido* representa el número de imagen a mostrar por el cliente cuando el edificio está construido y con más del 20% de la vida. 
El sprite *destruido* representa el edificio cuando tiene menos del 20% de la vida.
El sprite *base* se utiliza cuando un edificio requiere componer su imagen a partir de dos sprites. Los parámetros `x` e `y` en los tres casos permiten desplazar el sprite para hacerlo coincidir con el sprite base.
Un valor de -1 en el ID de sprite deshabilita el dibujado de este sprite.
- **Sprite del botón de construcción**  
*Valor por defecto:* `"sprite_boton": -1`  
Indica cual es el número de imagen correspondiente al botón de construir.


Ejemplo de archivo `edificios.json`:

```json
[
    {
        "id": "centro_construccion",
        "nombre": "Centro de Construcción",
        "energia": 0,
        "costo": 0,
        "dimensiones": [3, 3],
        "tiempo_construccion": 1,
        "puntos_estructura": 3000,
        "sprite_construido": { "id": 2967 },
        "sprite_destruido": { "id": 2968 }
    },
    {
        "id": "refineria",
        "nombre": "Refinería",
        "descripcion": "Lugar donde las cosechadores almacenan la especia recolectada.",
        "metadata": { 
            "Energía req.": "400",
            "P. estructura": "500",
            "Capacidad": "5000"
        },
        "energia": 400,
        "costo": 500,
        "dimensiones": [3, 3],
        "puntos_estructura": 500,
        "capacidad_especia": 5000,
        "sprite_base": { "id": 2969 },
        "sprite_construido": {"id": 2970, "x": 5, "y": -27},
        "sprite_destruido": {"id": 2971, "x": 2, "y": -32},
        "sprite_boton": 4330
    }
]
```

### Archivo de ejércitos
El archivo de ejércitos es el que contiene toda la información requerida por
el servidor y los clientes sobre los distintos tipos de tropa. 

El mismo debe estar en formato JSON y contener, **únicamente** un objeto con 
dos atributos: `armas` y `unidades`.  
El atributo `armas` debe ser un arreglo de objetos con los siguientes atributos:

- **ID**  
*Valor por defecto:* `"id": "defecto"`  
Este es el identificador del *tipo* de arma. Debe ser único por cada 
clase de arma distinta. Este parámetro es obligatorio.
- **Daño**  
*Valor por defecto:* `"daño": "0"`  
El daño que hace el arma cada vez que es disparada.
- **Frecuencia de disparo**  
*Valor por defecto:* `"frecuencia": "1"`  
Cuántas veces por segundo se debe disparar el arma.
- **Bonificaciones por clase de tropa**  
*Valor por defecto:* `"bonificacion_tropa": {}`  
Debe ser un objeto donde cada atributo es el ID de una tropa definida en 
`unidades` y el valor es el daño extra a realizar (daño total = daño + 
bonificacion).
- **Bonficaciones por tipo de tropa**  
*Valor por defecto:* `"bonificacion_tipo_tropa": {}`  
Debe ser un objeto donde cada atributo es `vehiculo` o `unidad` para determinar
si la bonificación aplica a vehículos o a unidades, respectivamente; y el valor
el mismo es el daño extra a realizar.
- **Bonficaciones a edificios**  
*Valor por defecto:* `"bonificacion_edificios": 0`  
Indica el daño extra realizado por la tropa a los edificios.


El atributo `unidades` debe ser un arreglo de objetos con los siguientes
atributos:  

- **ID**  
*Valor por defecto:* `"id": "defecto"`  
Este es el identificador del *tipo* de tropa. Debe ser único por cada 
clase de tropa distinta.
- **Nombre**  
*Valor por defecto:* `"nombre": "-"`  
El nombre *legible por humanos* de la tropa, para mostrar en la interfaz gráfica.
- **Descripción**  
*Valor por defecto:* `"descripcion": "-"`  
Descripción de la tropa para mostrar en la interfaz gráfica. No debe superar los 100 caracteres.
- **Metadata**  
*Valor por defecto:* `"metadata": { }`  
Metadatos de la tropa, este campo sólo se utiliza para mostrar información extra al usuario (no afecta al juego). El mismo debe ser un objeto JSON donde la clave es un nombre de atributo y el valor debe ser el valor del mismo. Un ejemplo de esto sería lo siguiente: `{"Rango": "3 casillas"}`.
- **Casa**  
*Valor por defecto:* `"casa": ["atreides","harkonnen","ordos"]`  
Identificadores de las casas que pueden construir este tipo de edificio. Debe ser un arreglo que contenga al menos una y como máximo tres de las mencionadas en el valor por defecto. 
- **Requerimiento**  
*Valor por defecto:* `"requerimiento": [ ]`  
Indica qué edificios son requeridos para poder entrenar este tipo de tropa. Observar que los ID son strings que deben coincidir con los IDs definidos en el archivo de edificios.
- **Armas**  
*Valor por defecto:* `"id_arma": []`  
Arreglo con los ID de armas que posee la tropa. El ID debe coincidir con el indicado
en el atributo `armas` de este mismo archivo.
- **Rango**  
*Valor por defecto:* `"rango": 3`  
Rango de ataque de la tropa, en casillas.
- **Velocidad**  
*Valor por defecto:* `"velocidad": 16`  
Velocidad de la tropa, en km/h.
- **Tiempo de entrenamiento**  
*Valor por defecto:* `"tiempo de entrenamiento": 1.0`  
Tiempo de entrenamiento de la tropa en minutos. Observar que el valor es número de punto flotante.
- **Costo**  
*Valor por defecto:* `"costo": 0`  
Dinero requerido para entrenar la tropa.
- **Vida**  
*Valor por defecto:* `"vida": 0`  
Vida máxima de la tropa.
- **Sprite base, techo y disparo**  
*Valor por defecto:* `"sprite_base": 206`  
*Valor por defecto:* `"sprite_techo": -1`  
Estos sprites definen cómo se dibujará la tropa en los clientes.  
Sprite base define el primer sprite de una secuencia de sprites. Para el caso de los vehículos, este sprite define el primero de los 32 sprites de orientación del vehículo. Para el caso de las unidades, este sprite define el primero de las animaciones de caminar, disparar y fallecer.  
Sprite techo sólo se utiliza en el caso de los vehículos y define el primero de 32 sprites orientables que se dibujará sobre el techo del sprite base.
- **Sprite de descarga**  
*Valor por defecto:* `"sprite_descarga": -1`  
Para las unidades que recolectan especia, este sprite indica cual es el primero de la animación de descarga de la especia en las refinerías.
- **Sprite del botón de entrenamiento**  
*Valor por defecto:* `"sprite_boton": -1`  
Indica cual es el número de imagen correspondiente al botón de entrenar.
- **Tipo de unidad**  
*Valor por defecto:* `"tipo": "unidad"`  
Indica si es una unidad o un vehículo. Los valores admitidos son `"unidad"` y `"vehiculo"`.

Ejemplo de archivo `ejercitos.json`:

```json
{
    "armas": [
        {
            "id": "lanza_misiles",
            "daño": 4,
            "frecuencia": 1,
            "bonificacion_tropa": {
                "infanteria ligera": 4,
                "infanteria pesada": 4
            }
        },
        {
            "id": "rifle_asalto",
            "daño": 8,
            "frecuencia": 1,
            "bonificacion_tipo_tropa": { "vehiculo": 2 },
            "bonificacion_edificios": 2
        }
    ],
    "unidades": [
        {
            "id":"infanteria pesada",
            "nombre":"Infantería pesada",
            "descripcion": "Efectivas contra unidades blindadas, no así contra la infantería.",
            "metadata": {
                "Vida": "80",
                "Rango": "3 casillas",
                "Velocidad": "12 km/h"
            },
            "casa":["atreides","harkonnen","ordos"],
            "requerimiento":["cuartel"],
            "id_arma":["lanza_misiles"],
            "rango":3,
            "velocidad":12,
            "tiempo de entrenamiento":1.3,
            "costo":70,
            "vida":80,
            "sprite_boton": 4273,
            "sprite_base": 458,
            "tipo": "unidad"
        },
        {
            "id":"fremen",
            "nombre":"Fremen",
            "descripcion": "Guerreros nativos del planeta Arrakis que saben como moverse en el desierto.",
            "metadata": {
                "Vida": "50",
                "Rango": "3 casillas",
                "Velocidad": "20 km/h"
            },
            "casa":["atreides"],
            "requerimiento":["cuartel","palacio"],
            "id_arma":["rifle_asalto", "lanza_misiles"],
            "rango":3,
            "velocidad":20,
            "tiempo de entrenamiento":2,
            "costo":100,
            "vida":50,
            "sprite_boton": 4293,
            "sprite_base": 710,
            "tipo": "unidad"
        }
    ]
}
```
