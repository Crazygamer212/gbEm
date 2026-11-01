#pragma once
#include "cpu/rf.h"


namespace Cpu {
    class IDU {
        RegisterFile* pRF;
    public:
        IDU(RegisterFile* RegisterFile);
        ~IDU();
        void clock();
    };
}