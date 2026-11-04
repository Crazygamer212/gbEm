#include "cpu/idu.h"
#include "bus/controll.h"


Cpu::IDU::IDU(RegisterFile* pRegisterFile, Bus::Addr* pABus, Bus::Ctrl* pCBus) : pRF(pRegisterFile), pABus(pABus), pCBus(pCBus) {

}

Cpu::IDU::~IDU() {

}

void Cpu::IDU::clock() {

}

void Cpu::IDU::nextPC() {
    pABus->write(pRF->PC++);
    pCBus->setState(Bus::State::read);
}