#include "cpu/alu.h"

#include "bus/data.h"
#include "cpu/rf.h"
#include <sys/types.h>

Cpu::ALU::ALU(Bus::Data *pDBus, RegisterFile *pRF) : pDBus(pDBus), pRF(pRF) {

}

Cpu::ALU::~ALU() {

}

void Cpu::ALU::clock() {
//NI - requires T-cycles
}

void Cpu::ALU::move(int to, int from) {
    u_int8_t &v = *pRF->addr8bmap(to);
    v = *pRF->addr8bmap(from);
}

#define operation(name,code)  \
    void Cpu::ALU::name##V(u_int8_t &v) { code } \
//    void Cpu::ALU::name##R(int r) { name##V(*pRF->addr8bmap(r)); } \


operation(add, 
pRF->A += v;
pRF->setH(?);
pRF->setC(?);

pRF->setZ(pRF->A==0);
pRF->setN(0);
)

operation(adc, 
pRF->A += v + pRF->getC();
pRF->setH(?);
pRF->setC(?);

pRF->setZ(pRF->A==0);
pRF->setN(0);
)

operation(sub, 
pRF->A -= v;
pRF->setH(?);
pRF->setC(?);

pRF->setZ(pRF->A==0);
pRF->setN(1);
)

operation(sbc, 
pRF->A -= v + pRF->getC();
pRF->setH(?);
pRF->setC(?);

pRF->setZ(pRF->A==0);
pRF->setN(1);
)

operation(cp, 
u_int8_t Z = pRF->A - v;
pRF->setH(?);
pRF->setC(?);

pRF->setZ(pRF->A==0);
pRF->setN(1);
)

//INC
//DEC

operation(and, 
pRF->A = pRF->A & v;
pRF->setZ(pRF->A==0);
pRF->setN(0);
pRF->setH(1);
pRF->setC(0);
)

operation(or, 
pRF->A = pRF->A | v;
pRF->setZ(pRF->A==0);
pRF->setN(0);
pRF->setH(0);
pRF->setC(0);
)

operation(xor, 
pRF->A = pRF->A ^ v;
pRF->setZ(pRF->A==0);
pRF->setN(0);
pRF->setH(0);
pRF->setC(0);
)


