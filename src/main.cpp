#include <cstddef>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <chrono>



#include "bus/address.h"
#include "bus/controll.h"
#include "bus/data.h"
#include "cpu/cpu.h"
#include "graphics/ppu.h"
#include "memory/memory.h"

#include "graphics/screen.h"


// TODO
//TODO
// TODO
// CHECK TESTS FOR LITTLE ENDIANESS

// clock cycle:
/*
    CPU
    MEM
    ...
    BUSSES
*/

int main(int argc, char* argv[]) {
    std::streampos size;
    u_int8_t *pROMData;

    std::fstream fROMBin;
    fROMBin.open(argv[1], std::ios::binary | std::ios::in | std::ios::ate);

    size = fROMBin.tellg();
    if (size == -1) {throw std::runtime_error("couldn't open file");}
    pROMData = new u_int8_t[size];

    fROMBin.seekg (0, std::ios::beg);
    fROMBin.read(reinterpret_cast<char*>(pROMData), size);

    fROMBin.close();


    Cpu::Bus::Data cpuDBus;
    Cpu::Bus::Addr cpuABus;
    Cpu::Bus::Ctrl cpuCBus;

    Cpu::Bus::Data memDBus;
    Cpu::Bus::Addr memABus;
    Cpu::Bus::Ctrl memCBus;

    Cpu::Cpu       cpu   (&cpuDBus, &cpuABus, &cpuCBus);
    Memory::Memory memory(&memDBus, &memABus, &memCBus, cpu.getIE(), cpu.getIF()); 


    memory.loadb(pROMData, (size_t)size);
    memDBus.clock(); memABus.clock(); memCBus.clock();
    
    Graphics::PPU ppu(  cpuDBus, cpuABus, cpuCBus,
                        memDBus, memABus, memCBus );

    long long clock = 0;
    while (true) {
        cpu.clock();

        memory.clock();

        ppu.clock();// 99.99 of the clock cycle runs here
        
        #ifndef NDEBUG
         cpuDBus.clock(); cpuABus.clock(); cpuCBus.clock(); //don't need clocks if not checking for multiple clocks
         memDBus.clock(); memABus.clock(); memCBus.clock(); 
        #endif



       #ifndef NDEBUG
        std::cout << std::to_string(clock++) 
        << "|  opcode: " << std::to_string(cpu.getOpcode())
        << std::endl;
       #endif
    }
    SDL_Quit();
    delete[] pROMData;
}