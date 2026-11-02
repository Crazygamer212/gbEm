#include "cpu/cu.h"

#include <stdexcept>
#include <sys/types.h>


#include "cpu/rf.h"
#include "bus/address.h"
#include "bus/data.h"


Cpu::ControllUnit::ControllUnit(RegisterFile* pRF, Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus) : pRF(pRF), pDBus(pDBus), pABus(pABus), pCBus(pCBus) {

}
Cpu::ControllUnit::~ControllUnit() {

}

// TODO: STATES!
/*  have the CU have different states for M cycles. it stores OPcode untill it is done.
 *  This allows it to wait for memory fetches and have states per clock while knowing what it's running
 */ 
void Cpu::ControllUnit::clock() {

    if (this->Mc == 1) {
        this->pRF->IR = /*DATA bus value*/this->pDBus->read();
    }

    //decode + exec (1 cycle)
    int opcode = this->pRF->IR;
    switch (opcode & 0xC0) {
        case 0x40: { //0b01_xxx_xxx  2[1] cycles  -  Done  
            u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
            u_int8_t &from = *pRF->addr8bmap((opcode & 0b00000111));
            pRF->PC ++;
            to = from;
            finIR = true;
            return;
        }
        case 0x00: { //0b00_xxx_110(HL)  4[3] cycles
            // NEED TO FETCH FROM RAM - SET finIR to false
            u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
            u_int16_t &from = pRF->HL;
            if ((opcode & 0b00000111) != 0b110) { throw std::runtime_error("Unimplemented opcode 0b01_xxx_??? (01) "); }
            pRF->PC ++;

        }
    }


    if (finIR) {
        this->Mc = 1;
        // Set pRF->IR to fetch_ram(PC)
    }
}