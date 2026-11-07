#include <sys/types.h>

#include "cpu/cpu.h"

#include "bus/controll.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"









// ------ CPU ------
Cpu::Cpu::Cpu(Bus::Data* pDBus, Bus::Addr* pABus, Bus::Ctrl* pCBus) : pDBus(pDBus), pABus(pABus), pCBus(pCBus) {
    pRF = new RegisterFile();
    //pDBus = new Bus::Data();
    //pABus = new Bus::Addr();
    //pCBus = new Bus::Ctrl();

    pALU = new ALU(this->pDBus, pRF);
    pIDU = new IDU(pRF, this->pABus, this->pCBus);
    pCU  = new ControllUnit(pRF, this->pABus, this->pDBus, this->pCBus, pALU, pIDU);

    pRF->PC=101;
    pABus->write(pRF->PC-1);
    pCBus->setState(Bus::State::read);
}

Cpu::Cpu::~Cpu() {
    delete(pCU);
    delete(pALU);
    delete(pIDU);

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
    pIDU->clock();
    pALU->clock();

    pCU->clock(); // the only thing that does
}
