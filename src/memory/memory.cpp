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

Memory::Memory::Memory(Bus::Data *pDBus, Bus::Addr *pABus, Bus::Ctrl *pCBus, u_int8_t& IE, u_int8_t& IF) : pDBus(pDBus), pABus(pABus), pCBus(pCBus), IEReg(&IE), IFReg(&IF) {

}

Memory::Memory::~Memory() {

}

u_int8_t& Memory::Memory::operator[](u_int16_t addr) {
    if (ramLocked) {
        if (addr < 0xFF80  &&  addr >= 0xFFFF) {
            throw std::runtime_error("NI erronous results");
        }
    }

    if ( addr < 0x4000 ) { // 16 KiB ROM bank 00
        return this->pRomBnkNN[0][addr];
    } else if ( addr < 0x8000 ) { // 16 KiB ROM Bank 01–NN
        return this->pRomBnkNN[1][addr-0x4000]; // https://gbdev.io/pandocs/MBC3.html
    } else if ( addr < 0xA000 ) { // 8 KiB Video RAM (VRAM)
        return this->VRAM[addr-0x8000];
    } else if ( addr < 0xC000 ) { // 8 KiB External RAM  --  From cartridge, switchable bank if any
        return this->ERAM[addr-0xA000];
    } else if ( addr < 0xD000 ) { // 4 KiB Work RAM (WRAM)
        return this->WRAM[addr-0xC000];
    } else if ( addr < 0xE000 ) { // 4 KiB Work RAM (WRAM)
        return this->WRAM1[addr-0xD000]; // color gameboy switchable
    } else if ( addr < 0xFE00 ) { // Echo RAM (mirror of C000–DDFF)
        return (*this)[addr-(0xFE00+0xE000)];
    } else if ( addr < 0xFEA0 ) { // Object attribute memory (OAM)
        return this->OAM[addr-0xFE00];
    } else if ( addr < 0xFF00 ) { // Not usable
        return *static_cast<u_int8_t*>(nullptr);
    } else if ( addr < 0xFF80 ) { // I/O Registers
        if (addr == 0xff0f)  { return *IFReg; }
        return this->IORgs[addr-0xFF00];
    } else if ( addr < 0xFFFF ) { // High RAM (HRAM)
        return this->HRAM[addr-0xFF80];
    } else if (addr == 0xFFFF) { // Interrupt Enable register (IE)
        return *IEReg;
    }
    throw std::runtime_error("How did you get here? OOB memory");
}



void Memory::Memory::clock() {
    if (ramLocked) {
        --ramLocked; // copy from $XX00-$XX9F   ;XX = $00 to $DF
                     // to $FE00-$FE9F
                     // writing XX to $FF46 starts the transfer
                     // then divide by $100
        
    }
    switch (pCBus->getState()) {
        case Bus::State::read: {
            pDBus->write( (*this)[pABus->read()] );
            break;
        }
        case Bus::State::write: {
            if (pABus->read() <=)
            (*this)[pABus->read()] = pDBus->read();
            break;
        }
        case Bus::State::idle: {
            break;// lazy like me    d-(^_^)z
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
}


u_int8_t* Memory::Memory::getVRAM() {
    return VRAM;
}
u_int8_t* Memory::Memory::getOAM() {
    return OAM;
}