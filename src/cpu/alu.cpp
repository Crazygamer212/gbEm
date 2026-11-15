#include "cpu/alu.h"

#include "bus/data.h"
#include "cpu/rf.h"
#include <sys/types.h>


u_int8_t add4bit(u_int8_t A, u_int8_t B, u_int8_t c) {
    A &= 0xf;
    B &= 0xf;
    c &= 0b1;

    return A+B+c;
}

u_int8_t sub4bit(u_int8_t A, u_int8_t B, u_int8_t c) {
    A &= 0xf;
    B &= 0xf;
    c &= 0b1;

    return (A-B-c) & 0b11111;
}



Cpu::ALU::ALU(Bus::Data *pDBus, RegisterFile *pRF) : pDBus(pDBus), pRF(pRF) {

}

Cpu::ALU::~ALU() {

}

void Cpu::ALU::clock() {
//NI - requires T-cycles implementation
}

void Cpu::ALU::move(int to, int from) {
    u_int8_t &v = *pRF->addr8bmap(to);
    v = *pRF->addr8bmap(from);
}

#define operation(name,code)  \
    void Cpu::ALU::name##V(u_int8_t &v) { code } \
//    void Cpu::ALU::name##R(int r) { name##V(*pRF->addr8bmap(r)); } \


operation(add, 
u_int8_t lower_nib = add4bit(v, pRF->A, 0);
pRF->setH(lower_nib >> 4);
u_int8_t upper_nib = add4bit(v>>4, pRF->A>>4, pRF->getH());
pRF->setC(upper_nib >> 4);
pRF->A = (upper_nib&0xf) << 4  |  lower_nib&0xf;

pRF->setZ(pRF->A==0);
pRF->setN(0);
)

operation(adc, 
u_int8_t lower_nib = add4bit(v, pRF->A, pRF->getC());
pRF->setH(lower_nib >> 4);
u_int8_t upper_nib = add4bit(v>>4, pRF->A>>4, pRF->getH());
pRF->setC(upper_nib >> 4);
pRF->A = (upper_nib&0xf) << 4  |  lower_nib&0xf;

pRF->setZ(pRF->A==0);
pRF->setN(0);
)

operation(sub, 
u_int8_t lower_nib = sub4bit(pRF->A, v, 0);
pRF->setH(lower_nib >> 4);
u_int8_t upper_nib = sub4bit(pRF->A>>4, v>>4, pRF->getH());
pRF->setC(upper_nib >> 4);
//pRF->A = (upper_nib&0xf) << 4  |  lower_nib&0xf; //-- wont work if lower nibble needs carry and upper nibble fuffils
pRF->A -= v;

pRF->setZ(pRF->A==0);
pRF->setN(1);
)

operation(sbc,
u_int8_t old_c = pRF->getC();
u_int8_t lower_nib = sub4bit(pRF->A, v, pRF->getC());
pRF->setH(lower_nib >> 4);
u_int8_t upper_nib = sub4bit(pRF->A>>4, v>>4, pRF->getH());
pRF->setC(upper_nib >> 4);
//pRF->A = (upper_nib&0xf) << 4  |  lower_nib&0xf;
pRF->A -= v + old_c;

pRF->setZ(pRF->A==0);
pRF->setN(1);
)

operation(cp, 
u_int8_t lower_nib = sub4bit(pRF->A, v, 0);
pRF->setH(lower_nib >> 4);
u_int8_t upper_nib = sub4bit(pRF->A>>4, v>>4, pRF->getH());
pRF->setC(upper_nib >> 4);
//u_int8_t Z = pRF->A - v;  // implied

pRF->setZ(pRF->A==v); // Z == 0 
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




void Cpu::ALU::incV(u_int8_t &v) {
    u_int8_t lower_nib = add4bit(v, 1, 0);
    pRF->setH(lower_nib >> 4);
    u_int8_t upper_nib = add4bit(v>>4, 0, pRF->getH());
    pRF->setC(upper_nib >> 4);
    v = (upper_nib&0xf) << 4  |  lower_nib&0xf;

    pRF->setZ(v==0);
    pRF->setN(0);
};
void Cpu::ALU::decV(u_int8_t &v) {
    u_int8_t lower_nib = sub4bit(v, 1, 0);
    pRF->setH(lower_nib >> 4);
    u_int8_t upper_nib = sub4bit(v>>4, 0, pRF->getH());
    pRF->setC(upper_nib >> 4);
    //v = (upper_nib&0xf) << 4  |  lower_nib&0xf; //-- wont work if lower nibble needs carry and upper nibble fuffils
    v--;

    pRF->setZ(v==0);
    pRF->setN(1);
};


void Cpu::ALU::addVV(u_int8_t &v1, u_int8_t &v2) {
    u_int8_t lower_nib = add4bit(v1, v2, 0);
    pRF->setH(lower_nib >> 4);
    u_int8_t upper_nib = add4bit(v1>>4, v2>>4, pRF->getH());
    pRF->setC(upper_nib >> 4);
    v1 = (upper_nib&0xf) << 4  |  lower_nib&0xf;

    pRF->setZ(v1==0);
    pRF->setN(0);
};
void Cpu::ALU::adcVV(u_int8_t &v1, u_int8_t &v2) {
    u_int8_t lower_nib = add4bit(v1, v2, pRF->getC());
    pRF->setH(lower_nib >> 4);
    u_int8_t upper_nib = add4bit(v1>>4, v2>>4, pRF->getH());
    pRF->setC(upper_nib >> 4);
    v1 = (upper_nib&0xf) << 4  |  lower_nib&0xf;

    pRF->setZ(v1==0);
    pRF->setN(0);
};

void Cpu::ALU::subVV(u_int8_t &v1, u_int8_t &v2) {
    u_int8_t lower_nib = sub4bit(v1, v2, 0);
    pRF->setH(lower_nib >> 4);
    u_int8_t upper_nib = sub4bit(v1>>4, v2>>4, pRF->getH());
    pRF->setC(upper_nib >> 4);
    //v = (upper_nib&0xf) << 4  |  lower_nib&0xf; //-- wont work if lower nibble needs carry and upper nibble fuffils
    v1-=v2;

    pRF->setZ(v1==0);
    pRF->setN(1);
};