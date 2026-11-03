#pragma once

#include "cpu/rf.h"
#include "bus/address.h"

namespace Cpu {
    class IDU {
        RegisterFile* pRF;
        Bus::Addr* pABus;
    public:
        IDU(RegisterFile* RegisterFile, Bus::Addr* pABus);
        ~IDU();
        void clock();
    };
}