#include "graphics/screen.h"

#include <stdexcept>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>




Graphics::Screen::Screen(int x_size, int y_size, int px_size) {
    /*TODO code here*/
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(SDL_GetError());
    }
    pWindow = SDL_CreateWindow(
        "My SDL3 App",
        x_size*px_size,
        y_size*px_size,
        0
    );
}
Graphics::Screen::Screen(int x_size, int y_size) : Screen(x_size, y_size, 1) {}

Graphics::Screen::~Screen() {
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}