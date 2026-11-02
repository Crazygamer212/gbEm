#pragma once

#include <stdexcept>
#include <sys/types.h>


#include "bus/controll.h"
#include "cpu/rf.h"
#include "bus/address.h"
#include "bus/data.h"

namespace Cpu {
    class ControllUnit {
        RegisterFile* pRF;
        Bus::Addr *pABus;
        Bus::Data *pDBus;
        Bus::Ctrl *pCBus;

        bool finIR = true;
        int Mc{1};

    public:
        ControllUnit(RegisterFile* pRF, Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus);
        ~ControllUnit();
        void clock();
    };
}