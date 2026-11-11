#include "bus/controll.h"
#include "cpu/cu.h"
#include <cstdint>
#include <stdexcept>
#include <sys/types.h>

// https://gb.insertcoin.dev/#base-20
// https://gekkio.fi/files/gb-docs/gbctr.pdf


void Cpu::ControllUnit::handle0b00(int trg, int src) {
    switch (trg) {
        case 0b000: { // NOP --- STOP --- JR r8 --- LD a16, SP
            switch (this->Mc) {
                case 1: {
                    if (src == 0) { this->Mc=1; return; } 
                    else if (src == 2) { throw std::runtime_error("STOP[0x02] opcode not implemented"); this->Mc=0;}
                    else if (src == 1) { throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented"); this->Mc++;}//5 clock instuction; i love breathing
                    //else if (src == 3) { throw std::runtime_error("JR[0x03] opcode not implemented"); this->Mc++;}
                    else {
                        this->Mc++;
                        pABus->write(pRF->PC++);
                        pCBus->setState(Bus::State::read);
                    }
                    break;
                }
                case 2: {
                    if (src == 1) {throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented. how did you get here?"); this->Mc++;}
                    else {
                        pRF->Temp = pDBus->read();
                        switch (src) {
                            case 3: {
                                this->Mc++;
                                break;
                            }
                            case 4: { // JR NZ
                                if (!pRF->getZ()) {
                                    this->Mc++;
                                } else {
                                    this->Mc=1;
                                }
                                break;
                            }
                            case 5: {
                                if (pRF->getZ()) {
                                    this->Mc++;
                                } else {
                                    this->Mc=1;
                                }
                                break;
                            }
                            case 6: { // JR NZ
                                if (!pRF->getC()) {
                                    this->Mc++;
                                } else {
                                    this->Mc=1;
                                }
                                break;
                            }
                            case 7: {
                                if (pRF->getC()) {
                                    this->Mc++;
                                } else {
                                    this->Mc=1;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    if (src == 1) {throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented. Why 5?? like i get why but WHYYY!?!");}
                    else { // jump
                        pRF->PC+=static_cast<int8_t>(pRF->Temp);
                    }
                    break;
                }
                default: {
                    throw std::runtime_error("Not implemented JR unreachable");
                }

            }
            break;
        }
        case 0b001: { // LD/ADD
            throw std::runtime_error("Not implemented");
            break;
        }
        case 0b010: {// LD r16, A --- LD A, r16
            throw std::runtime_error("Not implemented");
            break;
        }
        case 0b011: { // INC/DEC r16
            switch (this->Mc++) {
                case 1: {
                    switch (1 & trg) {
                        case 0:
                            ++*this->pRF->addr16bmap(0b11 & (trg>>2)); // bitwise& not nessecery
                            break;
                        case 1:
                            --*this->pRF->addr16bmap(0b11 & (trg>>2));
                            break;
                    }
                    break;
                } case 2: {
                    this->Mc=1;// burns a clock due to 2 IDU opps
                    break;
                }
            }
            break;
        }
        case 0b100:  // INC r8
        case 0b101: { // DEC r8
            u_int8_t *pr8 = pRF->addr8bmap(trg);
            switch (this->Mc++) {
                case 1: {
                    if (pr8==nullptr) {
                        pABus->write(pRF->HL);
                        pCBus->setState(Bus::State::read);
                        break;
                    } else {
                        1&trg ? --*pr8 : ++*pr8;
                        this->Mc=0;
                    }
                    break;
                } case 2: {
                    pABus->write(pRF->HL);
                    pDBus->write(pDBus->read() + 1);
                    pCBus->setState(Bus::State::write);
                    break;
                } case 3: {
                    this->Mc=1;
                    break;
                }
            }
            break;
        }
        case 0b110: { // LD
            switch (this->Mc++) {
                case 1: {
                    pABus->write(pRF->PC++);
                    pCBus->setState(Bus::State::read);
                    break;
                } case 2: {
                    u_int8_t* pr8 = pRF->addr8bmap(trg);
                    if (pr8 == nullptr) {
                        pABus->write(pRF->HL);
                        //pDBus->write(pDBus->read()); //implied
                        pCBus->setState(Bus::State::write);
                        break;
                    } else {
                        *pr8 = pDBus->read();
                        this->Mc = 1;
                    }
                    break;
                } case 3: {
                    this->Mc=1;
                }
            }
            break;
        }
        case 0b111: { // Rot+othrs
            throw std::runtime_error("Not implemented");
            break;
        }
    }
}