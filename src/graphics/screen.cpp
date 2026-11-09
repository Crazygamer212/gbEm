#include "graphics/screen.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_render.h"

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
    this->x_size = x_size;
    this->y_size = y_size;
    this->px_size = px_size;

    pRenderer = SDL_CreateRenderer(pWindow, nullptr);
}
Graphics::Screen::Screen(int x_size, int y_size) : Screen(x_size, y_size, 1) {}

Graphics::Screen::~Screen() {
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}


int Graphics::Screen::tick(int t) {
    SDL_PollEvent(&this->event);
    
    SDL_FRect rect;
    rect.h = this->px_size;
    rect.w = this->px_size;
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    for (int dx=0; dx<this->x_size; ++dx) {
        for (int dy=0; dy<this->y_size; ++dy) {
            rect.x = dx*px_size;
            rect.y = dy*px_size;

            SDL_SetRenderDrawColor(pRenderer,(dx*54 + 2*t) %255,(dy*33 - 3*t) %255,((dx+dy)*17 - t*7) %255,255);
            SDL_RenderFillRect(pRenderer, &rect);
        }
    }
    SDL_RenderPresent(pRenderer);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            return 1;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE) { return 1; }
        default:
            return 0;
    }
}