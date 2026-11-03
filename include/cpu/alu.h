#pragma once

#include "bus/data.h"
#include "cpu/rf.h"

namespace Cpu {
    class ALU {
        Bus::Data* pDBus;
        RegisterFile* pRF;
    public:
        ALU(Bus::Data* pDBus, RegisterFile* pRF);
        ~ALU();
        void clock();
        void move(int r1, int r2);
    };
}
