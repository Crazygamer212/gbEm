#pragma once

#include "bus/data.h"

namespace Cpu {
    class ALU {
        Bus::Data* pDBus;
    public:
        ALU(Bus::Data* pDBus);
        ~ALU();
        void clock();
    };
}
