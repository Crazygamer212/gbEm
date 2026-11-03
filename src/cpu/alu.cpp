#include "cpu/alu.h"

#include "bus/data.h"
#include "cpu/rf.h"
#include <sys/types.h>

Cpu::ALU::ALU(Bus::Data *pDBus, RegisterFile *pRF) : pDBus(pDBus), pRF(pRF) {

}

Cpu::ALU::~ALU() {

}

void Cpu::ALU::clock() {

}

void Cpu::ALU::move(int from, int to) {
    u_int8_t &v = *pRF->addr8bmap(to);
    v = *pRF->addr8bmap(from);
}