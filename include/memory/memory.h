#pragma once

#include "bus/address.h"
#include "bus/data.h"
#include "bus/controll.h"

#include <sys/types.h>

namespace Bus = Cpu::Bus;

namespace Memory {
    class Memory {
        u_int8_t bank_count{0};
        u_int8_t (*pRomBnkNN)[0x4000];
        u_int8_t VRAM[0x4000];
        u_int8_t ERAM[0x4000]; // External ram
        u_int8_t WRAM[0xE000-0xC000];
        u_int8_t WRAM1[0xE000-0xC000];
        u_int8_t OAM[0xFEA0-0xFE00];
        u_int8_t IORgs[0xFF80-0xFF00];
        u_int8_t HRAM[0xFFFF-0xFF80];
        u_int8_t *IEReg; // 0xFFFF-0xFFFE

        Bus::Addr *pABus;
        Bus::Data *pDBus;
        Bus::Ctrl *pCBus;

        u_int8_t RAW[0xffff]; // temporary

    public:
        Memory(Bus::Data *pDBus, Bus::Addr *pABus, Bus::Ctrl *pCBus);
        ~Memory();

        void clock();
        void loadb(u_int8_t *pData, size_t size);

        u_int8_t& operator[](u_int16_t addr);
    };
};

