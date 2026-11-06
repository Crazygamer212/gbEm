#include "cpu/cu.h"

#include <cstddef>
#include <stdexcept>
#include <sys/types.h>


#include "cpu/alu.h"
#include "cpu/idu.h"
#include "cpu/rf.h"
#include "bus/address.h"
#include "bus/data.h"
#include "bus/controll.h"


Cpu::ControllUnit::ControllUnit(RegisterFile *pRF, Bus::Addr *pABus, Bus::Data *pDBus, Bus::Ctrl *pCBus, ALU *pALU, IDU *pIDU)
 : pRF(pRF), pDBus(pDBus), pABus(pABus), pCBus(pCBus), pALU(pALU), pIDU(pIDU) {
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
        pRF->IR = this->pDBus->read();
    }

    //decode + exec (1 cycle)
    const int opcode = pRF->IR;

    //Op trg, src     E.g.  LD B, C  :  LD 000, 001  :  01_000_001  :  B = C
    int src = (opcode & 0b00000111);
    int trg = (opcode & 0b00111000) >> 3;

    u_int8_t* regT = pRF->addr8bmap(trg);
    u_int8_t* regS = pRF->addr8bmap(src);
    switch (opcode & 0xC0) { // 2 MSB
        case 0x40: { //0b01_trg_src  LD
            switch (this->Mc) {
                case 1: {
                    if (regT == nullptr && nullptr == regS) {
                        throw std::runtime_error("HALT");
                    } else if (regT == nullptr) {
                        // Mem[HL] = regS;
                        pABus->write(pRF->HL);
                        pCBus->setState(Bus::State::write);
                        pDBus->write(*regS);
                        this->Mc++;
                    } else if (regS == nullptr) {
                        // regT = Mem[HL];
                        pABus->write(pRF->HL);
                        pCBus->setState(Bus::State::read);
                        this->Mc++;
                    } else {
                        *regT = *regS;
                        this->Mc = 1;
                    }
                    break;
                }
                case 2: {
                    if (regS == nullptr) {
                        *regT = pDBus->read();
                        this->Mc = 1;
                    } else if (regT == nullptr) {
                        this->Mc = 1;// burn a clock to write to memory
                    }
                    break;
                }
            }; 
            break;// opcode break
        }
        case 0x80: { //0b10_xxx_xxx  ALU
            switch (this->Mc) {
                case 1: {
                    if (regS == nullptr) {
                        this->Mc++;
                        pABus->write(pRF->HL);
                        pCBus->setState(Bus::State::read);
                        break;
                    }
                    switch (trg) { // trg is operation type for 0b10 ___ 110
                        case 0b000: { // ADD
                            pALU->addV(*regS);
                            break;
                        }
                        case 0b001: { // ADC 
                            pALU->adcV(*regS);
                            break;
                        }
                        case 0b010: { // SUB
                            pALU->subV(*regS);
                            break;
                        }
                        case 0b011: { // SBC
                            pALU->sbcV(*regS);
                            break;
                        }

                        case 0b100: { // AND
                            pALU->andV(*regS);
                            break;
                        }
                        case 0b101: { // XOR
                            pALU->xorV(*regS);
                            break;
                        }
                        case 0b110: { // OR
                            pALU->orV(*regS);
                            break;
                        }
                        case 0b111: { // CP
                            pALU->cpV(*regS);
                            break;
                        }
                    }
                    this->Mc=1;
                    break;
                }
                case 2: {
                    u_int8_t tempData = pDBus->read();
                    switch (trg) { // trg is operation type for 0b10 ___ 110
                        case 0b000: { // ADD
                            pALU->addV(tempData);
                            break;
                        }
                        case 0b001: { // ADC 
                            pALU->adcV(tempData);
                            break;
                        }
                        case 0b010: { // SUB
                            pALU->subV(tempData);
                            break;
                        }
                        case 0b011: { // SBC
                            pALU->sbcV(tempData);
                            break;
                        }

                        case 0b100: { // AND
                            pALU->andV(tempData);
                            break;
                        }
                        case 0b101: { // XOR
                            pALU->xorV(tempData);
                            break;
                        }
                        case 0b110: { // OR
                            pALU->orV(tempData);
                            break;
                        }
                        case 0b111: { // CP
                            pALU->cpV(tempData);
                            break;
                        }
                    }
                    this->Mc=1;
                    break;
                }
            }
            break;
        }
        default: {
            throw std::runtime_error("Unimplemented");
        }
    }

    if (this->Mc == 1) {
        // Set pRF->IR to fetch_ram(PC) - done at start of next clock
        pIDU->nextPC(); // continue next clock
    }
}