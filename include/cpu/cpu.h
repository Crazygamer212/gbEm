#pragma once

#include <sys/types.h>

#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"


namespace Cpu {

    class Cpu {
        RegisterFile* pRF;
        Bus::Data* pDBus;
        Bus::Addr* pABus;

        ALU* pALU;
        IDU* pIDU;

        int timer;

    public:
        Cpu(/*SoC DBus, SoC ABus*/);
        ~Cpu();
        
        void clock();
        void iclock();
    };

}
