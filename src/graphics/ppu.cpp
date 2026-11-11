#include "graphics/ppu.h"

#include <stdexcept>
#include <sys/types.h>

#include "graphics/screen.h"


Graphics::PPU::PPU(u_int8_t vram[]) : vram(vram),
#ifndef NDEBUG 
    oamScreen(64*8, 12*8, 3, "OAM window"),
#endif
screen(160, 144, 3, "main window")
{
    
}

void Graphics::PPU::drawTile(u_int8_t *t_data, int x, int y){
    return this->drawTile(t_data,x,y, screen);
}

void Graphics::PPU::drawTile(u_int8_t *t_data, int x, int y, Screen &screen){
    for (int i=0; i<8; ++i) {
        u_int8_t lower = t_data[(i*2)+0];
        u_int8_t upper = t_data[(i*2)+1];
        for (int px=0; px<8; ++px) {
            int dx = x-1-px, dy = y-16+i;
            if (dx<0 || dy<0  ||  dx>=screen.x_size || dy>=screen.y_size) { continue; }
            u_int8_t lowerb = 1 & (lower>>px);
            u_int8_t upperb = 1 & (upper>>px);
            screen.drawPX(dx,dy, (upperb<<1) | lowerb);
        }
    }
}


void Graphics::PPU::clock() {
#ifndef NDEBUG 
    for (int i=0; i<64; ++i) {
        for (int j=0; j<12; ++j) {
            this->drawTile(vram + (j*64 + i)*16 + 8000, i*8 + 8, j*8 + 16, oamScreen);
        }
    }
    int v = oamScreen.tick(0);
    if (v) { throw std::runtime_error("app_closed");}
#endif
    int c = screen.tick(0);
    if (c) { throw std::runtime_error("app_closed");}
}