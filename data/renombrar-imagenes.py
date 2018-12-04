#!/usr/bin/python3

import os
import sys

def main():
    '''
    Renombra las imágenes del Dune a su posición dentro del set.
    Uso:
     Si las imagenes están en imgs/{nombre-en-hexa}.bmp, se debe ejecutar como
     ./renombrar-imagenes.py imgs > script.sh

    Esto va a generar un script de bash que renombrará las imágenes. Para
    ejecutar este último:
    chmod +x script.sh
    ./script.sh

    ADV: Las imágenes se renombran in-place.
    '''
    if len(sys.argv) != 2:
        print('Uso: ./renombrar-imagenes.py <ruta a carpeta con imgs>')
        return 1

    _, _, archivos = next(os.walk(sys.argv[1]))

    ids = { int(s.split('.')[0], 16): s for s in archivos }

    for i, k in enumerate(sorted(ids.keys())):
        print("mv -f \"{}\" \"{}\"".format(os.path.join(sys.argv[1], ids[k]),
            os.path.join(sys.argv[1], "{:05d}.bmp".format(i))))

    return 0

exit(main())
