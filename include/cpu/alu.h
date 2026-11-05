#pragma once

#include "bus/data.h"
#include "cpu/rf.h"
#include <sys/types.h>

namespace Cpu {
    class ALU {
        Bus::Data* pDBus;
        RegisterFile* pRF;
    public:
        ALU(Bus::Data* pDBus, RegisterFile* pRF);
        ~ALU();
        void clock();
        void move(int to, int from);

        void addR(int r);
        void addV(u_int8_t v);

        void adcR(int r);
        void adcV(u_int8_t v);

        void subR(int r);
        void subV(u_int8_t v);

        void sbcR(int r);
        void sbcV(u_int8_t v);

        void andR(int r);
        void andV(u_int8_t v);

        void xorR(int r);
        void xorV(u_int8_t v);

        void orR(int r);
        void orV(u_int8_t v);

        void cpR(int r);
        void cpV(u_int8_t v);
    };
}
