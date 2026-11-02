#include "cpu/cu.h"

#include <stdexcept>
#include <sys/types.h>


#include "cpu/rf.h"
#include "bus/address.h"
#include "bus/data.h"


Cpu::ControllUnit::ControllUnit(RegisterFile *pRF, Bus::Addr *pABus, Bus::Data *pDBus) : pRF(pRF), pDBus(pDBus), pABus(pABus) {

}
Cpu::ControllUnit::~ControllUnit() {

}

// TODO: STATES!
/*  have the CU have different states for M cycles. it stores OPcode untill it is done.
 *  This allows it to wait for memory fetches and have states per clock while knowing what it's running
 */ 
void Cpu::ControllUnit::clock() {
    if (finIR) {
        //fetch new instruction
        finIR = false;
        return;
    }

    //decode + exec (1 cycle)
    int opcode = this->pRF->IR;
    switch (opcode & 0xC0) {
        case 0x40: { //0b01_xxx_xxx  -  Done
            u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
            u_int8_t &from = *pRF->addr8bmap((opcode & 0b00000111));
            pRF->PC ++;
            to = from;
            finIR = true;
            return;
        }
        case 0x00: { //0b00_xxx_110(HL)
            u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
            u_int16_t &from = pRF->HL;
            if ((opcode & 0b00000111) != 0b110) { throw std::runtime_error("Unimplemented opcode 0b01_xxx_??? (01) "); }
            pRF->PC ++;

        }

    }
}