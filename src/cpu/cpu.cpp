#include <sys/types.h>

#include "cpu/cpu.h"

#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"






void Cpu::Bus::Data::clock() {

}
void Cpu::Bus::Addr::clock() {

}






// ------ CPU ------
Cpu::Cpu::Cpu() {
    pRF = new RegisterFile();
    pDBus = new Bus::Data();
    pABus = new Bus::Addr();

    pALU = new ALU(pDBus);
    pIDU = new IDU(pRF);
}

Cpu::Cpu::~Cpu() {
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
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
}
