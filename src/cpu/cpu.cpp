#include <sys/types.h>

#include "cpu/cpu.h"

#include "bus/controll.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"








void Cpu::Bus::Addr::clock() {
    //written_this_clock = false;
}






// ------ CPU ------
Cpu::Cpu::Cpu() {
    pRF = new RegisterFile();
    pDBus = new Bus::Data();
    pABus = new Bus::Addr();
    pCBus = new Bus::Ctrl();

    pALU = new ALU(pDBus, pRF);
    pIDU = new IDU(pRF, pABus);
    pCU  = new ControllUnit(pRF, pABus, pDBus, pCBus, pALU, pIDU);
}

Cpu::Cpu::~Cpu() {
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
    delete(pCBus);
    delete(pRF);
}

void Cpu::Cpu::clock() { // CLK 4MHz
    if (timer % 4 == 0) {
        timer = 0;
        this->iclock(); // PHI 1MHz
    }
    timer++;
}

void Cpu::Cpu::iclock() {
    pRF->clock();
    pDBus->clock();
    pABus->clock();
    pIDU->clock();
    pALU->clock();

    pCU->clock(); // the only thing that does
}
