#pragma once

#include <stdexcept>
#include <sys/types.h>


#include "bus/controll.h"
#include "cpu/alu.h"
#include "cpu/idu.h"
#include "bus/address.h"
#include "bus/data.h"

namespace Cpu {
    class ControllUnit {
        //RegisterFile* pRF;
        Bus::Addr *pABus;
        Bus::Data *pDBus;
        Bus::Ctrl *pCBus;
        ALU *pALU;
        IDU *pIDU;

        bool finIR = true;
        int Mc{1};

    public:
        ControllUnit(/*RegisterFile* pRF ,*/ Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus, 
            ALU *pALU, IDU *pIDU);
        ~ControllUnit();
        void clock();
    };
}