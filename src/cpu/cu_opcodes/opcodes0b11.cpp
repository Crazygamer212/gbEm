#include "cpu/cu.h"

#include <stdexcept>
#include <sys/types.h>

#include "bus/controll.h"



void Cpu::ControllUnit::handle0b11(int trg, int src) {
    switch (src) {
        case 0b000: {
            switch (trg & 0b100) {
                case 0b000: {
                    switch (this->Mc++) {
                        case 1: {            
                            /* theretical cc here. next clock is easier due to no extra vars*/
                            pCBus->setState(Bus::State::idle);
                            break;
                        } case 2: {
                            switch (trg) {
                                case 0: { if (pRF->getZ()) { Mc=1; break; }; } // NC
                                case 1: { if (!pRF->getZ()) { Mc=1; break; }; }// Z
                                case 2: { if (pRF->getC()) { Mc=1; break; }; } // NC
                                case 3: { if (!pRF->getC()) { Mc=1; break; }; }// C
                            }
                            pABus->write(pRF->SP++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 3: {
                            pRF->Temp = pDBus->read();
                            pABus->write(pRF->SP++);
                            pCBus->setState(Bus::State::read);
                        } case 4: {
                            pRF->PC = pDBus->read()<<8 | pRF->Temp;
                        } case 5: {
                            this->Mc=1;
                        }
                    }
                    break;
                } case 0b100: {
                    throw std::runtime_error("NI 0b11 000 1xx");
                    break;
                }
            }
        } case 0b001: {
            switch (trg & 1) {
                case 0: { //POP RR
                    switch (this->Mc++) {
                        case 1: {
                            pABus->write(pRF->SP++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 2: {
                            *pRF->addr8bmapForSP(trg&0b110 | 1) = pDBus->read();
                            pABus->write(pRF->SP++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 3: {
                            *pRF->addr8bmapForSP(trg&0b110 | 0) = pDBus->read();
                            this->Mc=1;
                            break;
                        }
                    }
                } case 1: {
                    switch (trg &0b110) {
                        case 0b010:
                        case 0b000: {
                            switch (this->Mc++) {
                                case 1: {
                                    pABus->write(pRF->SP++);
                                    pCBus->setState(Bus::State::read);
                                    break;
                                } case 2: {
                                    pRF->Temp = pDBus->read();
                                    pABus->write(pRF->SP++);
                                    pCBus->setState(Bus::State::read);
                                    break;
                                } case 3: {
                                    pRF->PC = pDBus->read()<<8 | pRF->Temp;
                                    if (trg == 0b010) {/* IME = 1 */ throw std::runtime_error("NI IME"); } //TODO
                                    break;
                                } case 4: {
                                    this->Mc=1;
                                }
                            }
                        }
                    }
                    break;
                }
            }
            break;
        } case 0b010: {
            switch (src & 0b100) {
                case 0b000: { // JP
                    break;
                } case 0b100: { // LD/LDH
                    break;
                }
            }
            break;
        } case 0b011: {
            switch (trg) {
                case 2: 
                case 3:
                case 4:
                case 5: { throw std::runtime_error("bricking opcode executed"); }
                case 0: {
                    switch (this->Mc++) {
                        case 1: {
                            pABus->write(pRF->PC++);
                            pCBus->setState(Bus::State::read);
                            break;
                        } case 2: {
                            pABus->write(pRF->PC++);
                            pCBus->setState(Bus::State::read);
                            pRF->Temp = pDBus->read();
                            break;
                        } case 3: {
                            pRF->PC = (u_int16_t)pDBus->read()<<8 | (u_int16_t)pRF->Temp;
                        } case 4: {
                            this->Mc=1;
                            break;
                        }
                    }
                }
                case 1: {
                    switch (this->Mc++) {
                        case 1: {
                            pIDU->nextPC();
                            break;
                        } default: {
                            this->handleCB();
                            break;
                        }
                    }
                    throw std::runtime_error("NI CB prefix");
                }
                case 6:
                case 7: {
                    throw std::runtime_error("NI DI/EI");
                } 
            }
            break;
        } case 0b100: {
            switch (trg) {
                case 0b00:
                case 0b01:
                case 0b10:
                case 0b11: {
                    switch (this->Mc++) {
                        case 1: {
                            pIDU->nextPC();
                            break;
                        } case 2: {
                            pRF->Temp16 = pDBus->read();
                            pIDU->nextPC();
                            break;
                        } case 3: {
                            pRF->Temp16 = pDBus->read()<<8  |  (pRF->Temp16&0xff);
                            switch (trg) {
                                case 0: { if (pRF->getZ()) { Mc=1; break; }; } // NC
                                case 1: { if (!pRF->getZ()) { Mc=1; break; }; }// Z
                                case 2: { if (pRF->getC()) { Mc=1; break; }; } // NC
                                case 3: { if (!pRF->getC()) { Mc=1; break; }; }// C
                            }
                            break;
                        } case 4: {
                            pABus->write(--pRF->SP);
                            pCBus->setState(Bus::State::write);
                            pDBus->write(reinterpret_cast<u_int8_t*>(&pRF->PC)[0]);
                            break;
                        } case 5: {
                            pABus->write(--pRF->SP);
                            pCBus->setState(Bus::State::write);
                            pDBus->write(reinterpret_cast<u_int8_t*>(&pRF->PC)[1]);
                            pRF->PC = pRF->Temp16;
                            break;
                        } case 6: {
                            this->Mc=1;
                            break;
                        }
                    }
                    break;
                } default: { throw std::runtime_error("bricking opcode"); }
            }
            break;
        } case 0b101: {
            switch (trg & 1) {
                case 0: { // PUSH reg
                    switch (this->Mc++) {
                        case 1: {
                            // SP--   // theoretically - done later as syntax
                            pCBus->setState(Bus::State::idle);
                        }
                        case 2: {
                            pDBus->write(*pRF->addr8bmapForSP(trg | 0));
                            pABus->write(--pRF->SP);
                            pCBus->setState(Bus::State::write);
                        } case 3: {
                            pDBus->write(*pRF->addr8bmapForSP(trg | 1));
                            pABus->write(--pRF->SP);
                            pCBus->setState(Bus::State::write);
                        } case 4: {
                            pCBus->setState(Bus::State::idle);
                        }
                    }
                    break;
                } case 1: { // bricks and other boring stuff (CALL a16 -- call imidiate)
                    if (trg == 1) {
                        switch (this->Mc++) { //TODO check if this is backwards?
                            case 1: {
                                pABus->write(pRF->PC++);
                                pCBus->setState(Bus::State::read);
                                break;
                            } case 2: {
                                pABus->write(pRF->PC++);
                                pCBus->setState(Bus::State::read);
                                pRF->Temp=pDBus->read();
                                break;
                            } case 3: {
                                pRF->Temp16 = pDBus->read()<<8 | pRF->Temp;
                                //SP-- theoretical done instead in M5
                                pCBus->setState(Bus::State::idle);
                                break;
                            } case 4: {
                                pABus->write(--pRF->SP);
                                pDBus->write(reinterpret_cast<u_int8_t*>(&pRF->PC)[1]); //msB
                                pCBus->setState(Bus::State::write);
                                break;
                            } case 5: {
                                pABus->write(--pRF->SP);
                                pDBus->write(reinterpret_cast<u_int8_t*>(&pRF->PC)[0]); //lsB
                                pCBus->setState(Bus::State::write);
                                pRF->PC=pRF->Temp16;
                                break;
                            } case 6: {
                                this->Mc=1;
                                break;
                            }
                        }
                    } else {
                        throw std::runtime_error("bricking opcode called");
                    }
                    break;
                }
            }
            break;
        } case 0b110: { // ALU A,n8
            switch (this->Mc++) {
                case 1: {
                    pABus->write(pRF->PC++);
                    pCBus->setState(Bus::State::read);
                    break;
                } case 2: {
                    u_int8_t n8 = pDBus->read();
                    switch (trg) {
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
                    pRF->PC = trg;
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