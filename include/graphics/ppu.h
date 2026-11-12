#pragma once

#include <sys/types.h>

#include "graphics/screen.h"


namespace Graphics {
    class PPU {
        u_int8_t *vram;
        u_int8_t *oam;
        Screen screen;
    #ifndef NDEBUG 
        Screen oamScreen;
    #endif

    public:
        PPU(u_int8_t vram[], u_int8_t oam[]);

        void drawTile(u_int8_t *t_data, int x, int y);
        void drawTile(u_int8_t *t_data, int x, int y, Screen &screen);

        void clock();
    };
}