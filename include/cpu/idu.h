#pragma once

#include "bus/controll.h"
#include "cpu/rf.h"
#include "bus/address.h"
#include <sys/types.h>

namespace Cpu {
    class IDU {
        RegisterFile* pRF;
        Bus::Addr* pABus;
        Bus::Ctrl* pCBus;
    public:
        IDU(RegisterFile* RegisterFile, Bus::Addr* pABus, Bus::Ctrl* pCBus);
        ~IDU();
        void clock();
        void nextPC();
    };
}