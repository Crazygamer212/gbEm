#include "bus/address.h"

#include <sys/types.h>
#include <stdexcept>


void Cpu::Bus::Addr::clock() {
#ifndef NDEBUG
    this->written_this_clock = false;
#endif
}

u_int16_t Cpu::Bus::Addr::read() {
    return this->data;
}

void Cpu::Bus::Addr::write(u_int16_t data) {
#ifndef NDEBUG
    if(this->written_this_clock) {
        throw std::runtime_error("Address bus written to more than once this clock");
    }
    this->written_this_clock = true;
#endif
    this->data = data;
}