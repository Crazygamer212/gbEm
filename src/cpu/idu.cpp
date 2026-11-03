#include "cpu/idu.h"


Cpu::IDU::IDU(RegisterFile* pRegisterFile, Bus::Addr* pABus) : pRF(pRegisterFile), pABus(pABus) {

}

Cpu::IDU::~IDU() {

}

void Cpu::IDU::clock() {

}