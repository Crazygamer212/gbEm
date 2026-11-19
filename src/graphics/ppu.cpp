#include "graphics/ppu.h"

#include <stdexcept>
#include <sys/types.h>

#include "graphics/screen.h"


Graphics::PPU::PPU(
    Cpu::Bus::Data &cpuDBus, Cpu::Bus::Addr &cpuABus, Cpu::Bus::Ctrl &cpuCBus,
    Cpu::Bus::Data &memDBus, Cpu::Bus::Addr &memABus, Cpu::Bus::Ctrl &memCBus
) : cpuDBus(cpuDBus), cpuABus(cpuABus), cpuCBus(cpuCBus),
    memDBus(memDBus), memABus(memABus), memCBus(memCBus),
#ifndef NDEBUG 
    oamScreen(64*8, 12*8, 3, "OAM window"),
#endif
screen(160, 144, 3, "main window")
{ 
    mode=2;
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



// should be called 4 times per M cycle
void Graphics::PPU::clock() {
    // 0 : VRAM, OAM
    // 1 : VRAM, OAM
    // 2 : VRAM
    // 3 : None
    dot+=1;
    if (dot>=456) { dot = 0; LY++; }
    if (dot<=80) { mode = 2; }
    if (LY==154) { LY = 0; }
    if (LY>143)  { mode=1; }

    
}

/*
void Graphics::PPU::clock() {
#ifndef NDEBUG 
    for (int i=0; i<64; ++i) {
        for (int j=0; j<12; ++j) {
            this->drawTile(VRAM + (j*64 + i)*16, i*8 + 8, j*8 + 16, oamScreen);
        }
    }
    int v = oamScreen.tick(0);
    if (v) { throw std::runtime_error("app_closed");}
#endif
    for (u_int8_t t=0; t<(0xFEF0-0xFE00); t+=4) {
        //oam[t+4] //flags
        drawTile(VRAM + (OAM[t+2]*16), OAM[t+1], OAM[t+0]);
    }
    int c = screen.tick(0);
    if (c) { throw std::runtime_error("app_closed");}
}
*/

