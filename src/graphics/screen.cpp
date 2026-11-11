#include "graphics/screen.h"

#include <stdexcept>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>




Graphics::Screen::Screen(int x_size, int y_size, int px_size, const char *name) {
    /*TODO code here*/
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(SDL_GetError());
    }
    pWindow = SDL_CreateWindow(
        name,
        x_size*px_size,
        y_size*px_size,
        0
    );
    this->x_size = x_size;
    this->y_size = y_size;
    this->px_size = px_size;

    if (!pWindow) {
        throw std::runtime_error(SDL_GetError());
    }

    pRenderer = SDL_CreateRenderer(pWindow, nullptr);

    if (!pRenderer) {
        throw std::runtime_error(SDL_GetError());
    }
}
Graphics::Screen::Screen(int x_size, int y_size) : Screen(x_size, y_size, 1, "My SDL app") {}

Graphics::Screen::~Screen() {
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    //SDL_Quit();
}

void Graphics::Screen::drawPX(int x, int y, int c) {
    SDL_FRect rect;
    rect.h = this->px_size;
    rect.w = this->px_size;

    rect.x = x*px_size;
    rect.y = y*px_size;

    SDL_SetRenderDrawColor(pRenderer,c*10,c*75+30,c*20,255);
    SDL_RenderFillRect(pRenderer, &rect);
}


int Graphics::Screen::tick(int t) {
    SDL_PollEvent(&this->event);
    

    SDL_RenderPresent(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, 0, 30, 0, 255);
    SDL_RenderClear(pRenderer);


    switch (event.type) {
        case SDL_EVENT_QUIT:
            return 1;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE) { return 1; }
        default:
            return 0;
    }
}