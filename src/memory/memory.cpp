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


#define IN_RANGE(targ, lower, upper) (( targ >= lower ) & ( targ < upper ))



namespace Bus = Cpu::Bus;

Memory::Memory::Memory(Bus::Data *pDBus, Bus::Addr *pABus, Bus::Ctrl *pCBus) : pDBus(pDBus), pABus(pABus), pCBus(pCBus) {

}

Memory::Memory::~Memory() {

}

u_int8_t& Memory::Memory::operator[](u_int16_t addr) {
    if ( addr < 0x4000 ) { // 16 KiB ROM bank 00
        return this->pRomBnkNN[0][addr];
    } else if ( addr < 0x8000 ) { // 16 KiB ROM Bank 01–NN
        return this->pRomBnkNN[1][addr-0x4000]; // https://gbdev.io/pandocs/MBC3.html
    } else if ( addr < 0xA000 ) { // 8 KiB Video RAM (VRAM)
        return this->VRAM[addr-0xA000];
    } else if ( addr < 0xC000 ) { // 8 KiB External RAM  --  From cartridge, switchable bank if any
        return this->ERAM[addr-0xC000];
    } else if ( addr < 0xD000 ) { // 4 KiB Work RAM (WRAM)
        return this->WRAM[addr-0xD000];
    } else if ( addr < 0xE000 ) { // 4 KiB Work RAM (WRAM)
        return this->WRAM1[addr-0xE000]; // color gameboy switchable
    } else if ( addr < 0xFE00 ) { // Echo RAM (mirror of C000–DDFF)
        return (*this)[addr-(0xFE00+0xC000)];
    } else if ( addr < 0xFEA0 ) { // Object attribute memory (OAM)
        return this->OAM[addr-0xFEA0];
    } else if ( addr < 0xFF00 ) { // Not usable
        return *static_cast<u_int8_t*>(nullptr);
    } else if ( addr < 0xFF80 ) { // I/O Registers
        return this->IORgs[addr-0xFF80];
    } else if ( addr < 0xFFFF ) { // High RAM (HRAM)
        return this->HRAM[addr-0xFF80];
    } else { // Interrupt Enable register (IE)
        throw std::runtime_error("NI - IE ram");
    }
}



void Memory::Memory::clock() {
    switch (pCBus->getState()) {
        case Bus::State::read: {
            pDBus->write( (*this)[pABus->read()] );
            break;
        }
        case Bus::State::write: {
            (*this)[pABus->read()] = pDBus->read();
            break;
        }
        default: {
            throw std::runtime_error("controll state undefined");
        }
    }
}

//  https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
void Memory::Memory::loadb(u_int8_t *pData, size_t size) {
#ifndef NDEBUG 
    std::cout.write((const char*)(pData+0x0134) , 0xf);
    std::cout << std::endl;
#endif
    
    bank_count = 1 << *(pData+0x0148);
    this->pRomBnkNN = new u_int8_t[bank_count][0x4000];
    for (int i=0; i<(bank_count); i++) {
        for (int j=0; j<0x4000; ++j) {
            this->pRomBnkNN[i][j] = pData[j+ (i)*0x4000];
        }
    }
    return;
}