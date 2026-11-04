#include "cpu/rf.h"

#include <stdexcept>



void Cpu::RegisterFile::clock() {

}


u_int8_t* Cpu::RegisterFile::addr8bmap(int num) {
    switch (num & 0b111) {
        case 0b000: return &(this->B);
        case 0b001: return &(this->C);
        case 0b010: return &(this->D);
        case 0b011: return &(this->E);
        case 0b100: return &(this->H);
        case 0b101: return &(this->L);
        case 0b110: return nullptr; // (HL) is memory, not a register
        case 0b111: return &(this->A);
    }
    return nullptr;
}