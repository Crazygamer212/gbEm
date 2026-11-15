#include "bus/controll.h"
#include "cpu/cu.h"
#include <cstdint>
#include <stdexcept>
#include <sys/types.h>

void Cpu::ControllUnit::handleCB(int B2) {
    if (this->Mc == 2)  pRF->CBIR = B2;
    u_int8_t *pr8 = pRF->addr8bmap( pRF->CBIR & 0x07);
    if (pr8 == nullptr) {
        throw std::runtime_error("CB [HL] operations NI");
        this->Mc++;
        return;
    };

    u_int8_t t_bit = 1 << ( (B2>>3)&0b111 );
    switch (pRF->CBIR & 0xC0) {
        case 0x00: { //operations
            throw std::runtime_error("NI B2@opp");
            break;
        } case 0x40: { // bit r8
            pRF->setZ(t_bit & *pr8);
            break;
        } case 0x80: { // res r8
            *pr8 &= ~t_bit;
            break;
        } case 0xc0: { // set r8
            *pr8 |= t_bit;
            break;
        }
    }
    this->Mc=1;
}