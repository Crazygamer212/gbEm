#include "memory/memory.h"

#include "bus/address.h"
#include "bus/data.h"
#include "bus/controll.h"

#include <stdexcept>
#include <sys/types.h>

namespace Bus = Cpu::Bus;

Memory::Memory::Memory(Bus::Data *pDBus, Bus::Addr *pABus, Bus::Ctrl *pCBus) : pDBus(pDBus), pABus(pABus), pCBus(pCBus) {

}

Memory::Memory::~Memory() {

}

void Memory::Memory::clock() {
    switch (pCBus->getState()) {
        case Bus::State::read: {
            pDBus->write(this->RAW[pABus->read()]);
            break;
        }
        case Bus::State::write: {
            this->RAW[pABus->read()] = pDBus->read();
            break;
        }
        default: {
            throw std::runtime_error("controll state undefined");
        }
    }
}

void Memory::Memory::loadb(u_int8_t *pData) {
    for (int i=0; i<0x7fff; i++) {
        this->RAW[i] = pData[i];
    }
}