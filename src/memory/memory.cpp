#include "memory/memory.h"

#include "bus/address.h"
#include "bus/data.h"
#include "bus/controll.h"


#include <stdexcept>
#include <sys/types.h>
#include <cstddef>

#ifndef NDEBUG 
#include <iostream>
#endif

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

//  https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
void Memory::Memory::loadb(u_int8_t *pData, size_t size) {
//#ifndef NDEBUG 
    std::cout.write((const char*)(pData+0x0134) , 0xf);
    std::cout << std::endl;
//#endif
    for (int j=0; j<0x4000; j++) {
        this->RomBnk00[j] = pData[j];
    }
    
    int iter=0;
    size=size/0x4000 -1;
    bank_count = 1 << *(pData+0x0148);
    this->pRomBnkNN = new u_int8_t[bank_count-1][0x4000];
    for (int i=0; i<(bank_count-1); i++) {
        for (int j=0; j<0x4000; ++j) {
            this->pRomBnkNN[i][j] = pData[j+ (i+1)*0x4000];
        }
    }
    return;
}