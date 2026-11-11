#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


/*
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "My SDL3 App",
        800,
        600,
        0
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;
            }
        }

        // Update game logic
        // ...

        // Render
        SDL_Renderer *renderer = SDL_GetRenderer(window);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/


namespace Graphics {
    class Screen {
        SDL_Window *pWindow = nullptr;
        SDL_Renderer *pRenderer = nullptr;
        SDL_Event event;

    public:
        int px_size{0};
        int x_size{0};
        int y_size{0};
    public:
        Screen(int x_size, int y_size);
        Screen(int x_size, int y_size, int px_size, const char *name);
        ~Screen();

        void drawPX(int x, int y, int c);
        int tick(int t);
    };
}
