#include "bus/data.h"


#include <stdexcept>



void Cpu::Bus::Data::clock() {
    this->written_this_clock = false;
}

u_int8_t Cpu::Bus::Data::read() {
    return this->data;
}

void Cpu::Bus::Data::write(u_int8_t data) {
    if(this->written_this_clock) {
        throw std::runtime_error("Data bus written to more than once this clock");
    }
    this->data = data;
    this->written_this_clock = true;
}