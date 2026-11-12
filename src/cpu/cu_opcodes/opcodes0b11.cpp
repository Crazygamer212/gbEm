#include "bus/controll.h"
#include "cpu/cu.h"
#include <stdexcept>
#include <sys/types.h>


void Cpu::ControllUnit::handle0b11(int trg, int src) {
    switch (trg) {
        case 0b000: {
            throw std::runtime_error("NI 0b11");
            break;
        } case 0b001: {
            throw std::runtime_error("NI 0b11");
            break;
        } case 0b010: {
            switch (trg & 0b100) {
                case 0b000: { // JP
                    break;
                } case 0b100: { // LD/LDH
                    break;
                }
            }
            break;
        } case 0b011: {
            throw std::runtime_error("NI 0b11");
            break;
        } case 0b100: {
            throw std::runtime_error("NI 0b11");
            break;
        } case 0b101: {
            throw std::runtime_error("NI 0b11");
            break;
        } case 0b110: { // ALU A,n8
            switch (this->Mc++) {
                case 1: {
                    pABus->write(pRF->PC++);
                    pCBus->setState(Bus::State::read);
                    break;
                } case 2: {
                    u_int8_t n8 = pDBus->read();
                    switch (src) {
                        case 0b000: {
                            pALU->addV(n8);
                            break;
                        } case 0b001: {
                            pALU->adcV(n8);
                            break;
                        } case 0b010: {
                            pALU->subV(n8);
                            break;
                        } case 0b011: {
                            pALU->sbcV(n8);
                            break;
                        } case 0b100: {
                            pALU->andV(n8);
                            break;
                        } case 0b101: {
                            pALU->xorV(n8);
                            break;
                        } case 0b110: {
                            pALU->orV(n8);
                            break;
                        } case 0b111: {
                            pALU->cpV(n8);
                            break;
                        }
                    }
                    this->Mc=1;
                    break;
                }
            }
            break;
        } case 0b111: { // RST $
            switch (this->Mc++) {
                case 1: {
                    pABus->write(--pRF->SP);
                    pDBus->write(0xff & (pRF->PC>>8)); // msB(PC)
                    pCBus->setState(Bus::State::write);
                    break;
                } case 2: {
                    pABus->write(--pRF->SP);
                    pDBus->write(0xff & pRF->PC); // msB(PC)
                    pCBus->setState(Bus::State::write);
                    break;
                } case 3: {
                    pRF->PC = src;
                    this->Mc=1; // burn a clock to commit memory
                }
                default:
                    throw std::runtime_error("Mc out of bounds RST $");
            }
            break;
        }
        default:
            throw std::runtime_error(
                "Unimplemented 0b11: trg=" + std::to_string(trg) +
                ", src=" + std::to_string(src)
            );
    }

}