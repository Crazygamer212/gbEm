#include "bus/controll.h"
#include "cpu/alu.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include <cstdint>
#include <stdexcept>
#include <sys/types.h>

// https://gb.insertcoin.dev/#base-20
// https://gekkio.fi/files/gb-docs/gbctr.pdf


void Cpu::ControllUnit::handle0b00(int trg, int src) {
    switch (src) { //chnaged
        case 0b000: { // NOP --- STOP --- JR r8 --- LD a16, SP
            switch (this->Mc) {
                case 1: {
                    if (trg == 0) { this->Mc=1; return; } 
                    else if (trg == 2) { throw std::runtime_error("STOP[0x02] opcode not implemented"); this->Mc=1;}
                    else if (trg == 1) { throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented"); this->Mc++;}//5 clock instuction; i love breathing
                    //else if (src == 3) { throw std::runtime_error("JR[0x03] opcode not implemented"); this->Mc++;}
                    else {
                        this->Mc++;
                        pABus->write(pRF->PC++);
                        pCBus->setState(Bus::State::read);
                    }
                    break;
                }
                case 2: {
                    if (trg == 1) {throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented. how did you get here?"); this->Mc++;}
                    else {
                        pRF->Temp = pDBus->read();
                        switch (trg) {
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
                        if (this->Mc != 1) { pCBus->setState(Bus::idle); }
                    }
                    break;
                }
                case 3: {
                    if (trg == 1) {throw std::runtime_error("LD (a16), SP [0x01] opcode not implemented. Why 5?? like i get why but WHYYY!?!");}
                    else { // jump
                        pRF->PC+=static_cast<int8_t>(pRF->Temp);
                        this->Mc=1;
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
            u_int8_t *pR8l;
            u_int8_t *pR8h;
            switch (trg & 0b110) {
                case 0b000:
                case 0b010: {
                    pR8l = pRF->addr8bmap((trg&0b110) | 1);
                    pR8h = pRF->addr8bmap((trg&0b110) | 0);
                    break;
                } case 0b100: {
                    pR8l = reinterpret_cast<uint8_t*>(&pRF->HL);
                    pR8h = reinterpret_cast<uint8_t*>(&pRF->HL) + 1;
                    break;
                } case 0b110: {
                    pR8l = reinterpret_cast<uint8_t*>(&pRF->SP);
                    pR8h = reinterpret_cast<uint8_t*>(&pRF->SP) + 1;
                    break;
                }
            }
            switch (trg & 1) {
                case 0: { // LD rr, n16
                    switch (this->Mc++) {
                        case 1: {
                            pABus->write(pRF->PC++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 2: {
                            *pR8l = pDBus->read();
                            pABus->write(pRF->PC++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 3: {
                            *pR8h = pDBus->read();
                            this->Mc=1;
                            break;
                        }
                    }
                    break;
                } case 1: { // ADD HL, rr
                    switch (this->Mc++) {
                        case 1: {
                            pALU->addVV(*pR8l, pRF->L);
                            pCBus->setState(Bus::State::idle);
                            break;
                        } case 2: {
                            pALU->adcVV(*pR8h, pRF->H);
                            this->Mc=1;
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 0b010: {// LD r16, A --- LD A, r16
            auto choseR = [](RegisterFile *pRF, u_int8_t c) {
                switch (c) { 
                    case 0b000: {
                        return pRF->BC;
                    }  case 0b010: {
                        return pRF->DE;
                    }  case 0b100: {
                        return pRF->HL++;
                    }  case 0b110: {
                        return pRF->HL--;
                    }
                    default: {
                        throw std::runtime_error("OOB execution");
                    }
                }
            };
            switch (this->Mc++) {
                case 1: { // IM[R16] << A   // A << IM[R16]
                    switch (trg & 1) {
                        case 0: { // IM[R16] << A
                            pABus->write(choseR(pRF, trg & 0b110));
                            pCBus->setState(Bus::State::write);
                            pDBus->write(pRF->A);
                            break;
                        } case 1: { // A << IM[R16] 
                            pABus->write(choseR(pRF, trg & 0b110));
                            pCBus->setState(Bus::State::read);
                            break;
                        }
                    }
                    break;
                } case 2: {
                    switch (trg & 1) {
                        case 0: { // IM[R16] << A
                            break;
                        } case 1: { // A << IM[R16]
                            pRF->A = pDBus->read();
                            break;
                        }
                    }
                    this->Mc=1;
                    break;
                } default: throw std::runtime_error("OOB execution");
            }
            break;
        }
        case 0b011: { // INC/DEC r16
            switch (this->Mc++) {
                case 1: {
                    switch (1 & trg) {
                        case 0:
                            ++*this->pRF->addr16bmap(/*0b11 &*/ (src>>2)); // bitwise& not nessecery
                            break;
                        case 1:
                            --*this->pRF->addr16bmap(/*0b11 &*/ (src>>2));
                            break;
                    }
                    break;
                } case 2: {
                    this->Mc=1;// burns a clock due to 2 opps  //TODO add flags
                    break;
                }
            }
            break;
        }
        case 0b100:  // INC r8
        case 0b101: { // DEC r8  //TODO add flags with alu via alu.add() / alu.sub()
            u_int8_t *pr8 = pRF->addr8bmap(trg);
            switch (this->Mc++) {
                case 1: {
                    if (pr8==nullptr) {
                        pABus->write(pRF->HL);
                        pCBus->setState(Bus::State::read);
                        break;
                    } else {
                        (pALU->*(1&src ? &ALU::decV : &ALU::incV)) (*pr8);
                        //(1&src ? pALU->subV : pALU->addV)(*pr8);
                        this->Mc=1;
                    }
                    break;
                } case 2: {
                    pABus->write(pRF->HL);
                    pDBus->write(pDBus->read() + (1&src ? -1:1));
                    pCBus->setState(Bus::State::write);

                    pRF->setN(1&src);
                    pRF->setZ(pDBus->read() == 0);
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
                        pDBus->write(pDBus->read()); //implied
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
            switch (trg &0b100) {
                case 0b000: {
                    switch (trg & 0b11) {
                        case 0b00: { pALU->rlcV(pRF->A); }
                        case 0b01: { pALU->rrcV(pRF->A); }
                        case 0b10: { pALU->rlV(pRF->A); }
                        case 0b11: { pALU->rrV(pRF->A); }
                    }
                    break;
                } case 0b100: {
                    switch (trg & 0b11) {
                        case 0b00: { // DAA
                            throw std::runtime_error("NI as not documented in gbctr");
                            break;
                        } case 0b01: { // CPL
                            pRF->setN(1);
                            pRF->setH(1);
                            pRF->A = ~pRF->A; 
                            break;
                        } case 0b10: { // SCF
                            pRF->setC(1);
                            pRF->setH(0);
                            pRF->setN(0);
                            break;
                        } case 0b11: { // CCF
                            pRF->setH(0);
                            pRF->setN(0);
                            pRF->setC(!pRF->getC());
                            break;
                        }
                    }
                    break;
                }
            }
            this->Mc = 1;
            break;
        }
    }
}