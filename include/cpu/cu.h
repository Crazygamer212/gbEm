#pragma once

#include "cpu/rf.h"
#include "bus/address.h"
#include "bus/data.h"

namespace Cpu {
    class ControllUnit {
        RegisterFile* pRF;
        Bus::Addr *pABus;
        Bus::Data *pDBus;

        bool finIR = true;

    public:
        ControllUnit(RegisterFile* pRF, Bus::Addr *pABus, Bus::Data *pDBus);
        ~ControllUnit();
        void clock();
    };
}