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


    Cpu::Bus::Data DBus;
    Cpu::Bus::Addr ABus;
    Cpu::Bus::Ctrl CBus;

    Cpu::Cpu       cpu   (&DBus, &ABus, &CBus);
    Memory::Memory memory(&DBus, &ABus, &CBus, cpu.getIE(), cpu.getIF()); 


    memory.loadb(pROMData, (size_t)size);
    DBus.clock(); ABus.clock(); CBus.clock();
    
    Graphics::PPU ppu(memory.getVRAM(), memory.getOAM());

    long long clock = 0;
    while (true) {
#ifndef NDEBUG
        auto start_cpu = std::chrono::high_resolution_clock::now();
#endif
        cpu.clock();

#ifndef NDEBUG
        auto cpu_mem = std::chrono::high_resolution_clock::now();
#endif
        memory.clock();

#ifndef NDEBUG
        auto mem_ppu = std::chrono::high_resolution_clock::now();
#endif
        ppu.clock();
        
#ifndef NDEBUG
        auto mem_bus = std::chrono::high_resolution_clock::now();
        DBus.clock(); ABus.clock(); CBus.clock(); //don't need clocks if not checking for multiple clocks
#endif


#ifndef NDEBUG
        auto bus_end = std::chrono::high_resolution_clock::now();
#endif

#ifndef NDEBUG
        std::cout << std::to_string(clock++) 
        << "|  opcode: " << std::to_string(cpu.getOpcode())
        << std::endl
        << "| CPU " << std::chrono::duration_cast<std::chrono::microseconds>(cpu_mem-start_cpu).count()
        << std::endl
        << "| MEM " << std::chrono::duration_cast<std::chrono::microseconds>(mem_ppu-cpu_mem).count()
        << std::endl
        << "| PPU " << std::chrono::duration_cast<std::chrono::microseconds>(mem_bus-mem_ppu).count()
        << std::endl
        << "| BUS " << std::chrono::duration_cast<std::chrono::microseconds>(bus_end-mem_bus).count()
        << std::endl;
#endif
    }
    SDL_Quit();
    delete[] pROMData;
}