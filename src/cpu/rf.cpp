#include "cpu/rf.h"

#include <stdexcept>
#include <sys/types.h>



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

u_int16_t* Cpu::RegisterFile::addr16bmap(int num) {
    switch (num & 0b11) {
        case 0b00: return &(this->BC);
        case 0b01: return &(this->DE);
        case 0b10: return &(this->HL);
        case 0b11: return &(this->SP);
    }
    return nullptr;
}



void Cpu::RegisterFile::setZ(bool status) {
    if (status) {
        F |= 0b0001;
    } else {
        F &= ~0b0001;
    }
}
void Cpu::RegisterFile::setN(bool status) {
    if (status) {
        F |= 0b0010;
    } else {
        F &= ~0b0010;
    }
}
void Cpu::RegisterFile::setH(bool status) {
    if (status) {
        F |= 0b0100;
    } else {
        F &= ~0b0100;
    }
}
void Cpu::RegisterFile::setC(bool status) {
    if (status) {
        F |= 0b1000;
    } else {
        F &= ~0b1000;
    }
}

u_int8_t Cpu::RegisterFile::getZ() {
    return (F     ) & 0b1;
}
u_int8_t Cpu::RegisterFile::getN() {
    return (F >> 1) & 0b1;
}
u_int8_t Cpu::RegisterFile::getH() {
    return (F >> 2) & 0b1;
}
u_int8_t Cpu::RegisterFile::getC() {
    return (F >> 3) & 0b1;
}