#pragma once

#include <sys/types.h>

#include "bus/controll.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"


namespace Cpu {
    namespace Bus {
        void clockAll();
    }

    class Cpu {
        RegisterFile* pRF;
        Bus::Data* pDBus;
        Bus::Addr* pABus;
        Bus::Ctrl* pCBus;

        ALU* pALU;
        IDU* pIDU;
        ControllUnit* pCU;

        int timer;

    public:
        Cpu(Bus::Data* pDBus, Bus::Addr* pABus, Bus::Ctrl* pCBus);  /*SoC DBus, SoC ABus, Soc CBus*/
        ~Cpu();
        
        void clock();
        void iclock();
    };

}
