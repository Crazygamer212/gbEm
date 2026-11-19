#pragma once

#include <sys/types.h>

#include "graphics/screen.h"
#include "bus/address.h"
#include "bus/controll.h"
#include "bus/data.h"


namespace Graphics {
    class PPU {
        u_int8_t VRAM[0x4000];
        u_int8_t OAM[0xFEA0-0xFE00];
        
        Cpu::Bus::Data &cpuDBus;
        Cpu::Bus::Addr &cpuABus;
        Cpu::Bus::Ctrl &cpuCBus;

        Cpu::Bus::Data &memDBus;
        Cpu::Bus::Addr &memABus;
        Cpu::Bus::Ctrl &memCBus;

        Screen screen;
    #ifndef NDEBUG 
        Screen oamScreen;
    #endif


        int mode{0};
        int LY{0};
        int dot{0};

    public:
        PPU( Cpu::Bus::Data &cpuDBus, Cpu::Bus::Addr &cpuABus, Cpu::Bus::Ctrl &cpuCBus,
             Cpu::Bus::Data &memDBus, Cpu::Bus::Addr &memABus, Cpu::Bus::Ctrl &memCBus );

        void drawTile(u_int8_t *t_data, int x, int y);
        void drawTile(u_int8_t *t_data, int x, int y, Screen &screen);

        void clock();
    };
}