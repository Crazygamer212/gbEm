#include "cpu/cu.h"

#include <stdexcept>
#include <sys/types.h>


#include "bus/controll.h"
#include "cpu/alu.h"
#include "cpu/idu.h"
#include "bus/address.h"
#include "bus/data.h"


Cpu::ControllUnit::ControllUnit( Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus, ALU *pALU, IDU *pIDU)
 : pDBus(pDBus), pABus(pABus), pCBus(pCBus), pALU(pALU), pIDU(pIDU) {
    // shouldn't use pRF - deligate to ALU+IDU
}

Cpu::ControllUnit::~ControllUnit() {

}

// TODO: STATES!
/*  have the CU have different states for M cycles. it stores OPcode untill it is done.
 *  This allows it to wait for memory fetches and have states per clock while knowing what it's running
 */ 
void Cpu::ControllUnit::clock() {

    if (this->Mc == 1) {
        pRF->IR = /*DATA bus value*/this->pDBus->read();
    }

    //decode + exec (1 cycle)
    int opcode = pRF->IR;
    switch (opcode & 0xC0) { // 2 MSB
        case 0x40: { //0b01_xxx_xxx  2[1] cycles  -  Done 
            //this->Mc++; // not needed but implied 
            u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
            u_int8_t &from = *pRF->addr8bmap((opcode & 0b00000111));
            to = from; //TODO SHOULD BE ALU

            pRF->PC ++; //TODO Should be IDU
            this->Mc = 1;
            break;
        }
        case 0x00: { //0b00_xxx_110(HL)  4[3] cycles
            switch (this->Mc) {
            case 1: { // First clock - fetch register and set up bus
                u_int8_t &to   = *pRF->addr8bmap((opcode & 0b00111000) >> 3);
                u_int16_t &addr = pRF->HL;
                if ((opcode & 0b00000111) != 0b110) { throw std::runtime_error("Unimplemented opcode 0b01_xxx_??? (01) "); }
                ++pRF->PC;
                ++this->Mc;
                break;
            } case 2: {

            }
            }
        }
        default: {
            throw std::runtime_error("Unimplemented");
        }
    }

    if (this->Mc == 1) {
        // Set pRF->IR to fetch_ram(PC) - done at start of next clock
        pABus->write(pRF->PC);
        pCBus->setState(Bus::State::read);
    }
}