#include "bus/controll.h"
#include <stdexcept>



void Cpu::Bus::Ctrl::clock() {
    this->written_this_clock = false;
}

void Cpu::Bus::Ctrl::setState(Cpu::Bus::State state) {
    if (this->written_this_clock) {
        throw std::runtime_error("controll bus written to again on same clock");
    }
    this->state = state;
    this->written_this_clock = true;
}

Cpu::Bus::State Cpu::Bus::Ctrl::getState() {
    return this->state;
}