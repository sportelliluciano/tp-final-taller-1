#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cliente/a.h"

int main(int argc, char *argv[]) {
    A a;
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "Hola cliente" << std::endl;
    SDL_Quit();
    return 0;
}
