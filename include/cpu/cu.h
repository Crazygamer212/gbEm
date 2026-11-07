#pragma once

#include <stdexcept>
#include <sys/types.h>


#include "cpu/alu.h"
#include "cpu/idu.h"
#include "cpu/rf.h"

#include "bus/address.h"
#include "bus/data.h"
#include "bus/controll.h"

namespace Cpu {
    class ControllUnit {
        RegisterFile* pRF;
        Bus::Addr *pABus;
        Bus::Data *pDBus;
        Bus::Ctrl *pCBus;
        ALU *pALU;
        IDU *pIDU;

        bool finIR = true;
        int Mc{1};

    public:
        ControllUnit(RegisterFile* pRF , Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus, 
            ALU *pALU, IDU *pIDU);
        ~ControllUnit();

        void handle0b00(int trg, int src);
        void handle0b11(int trg, int src);

        void clock();
    };
}