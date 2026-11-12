#include <cstddef>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sys/types.h>



#include "bus/address.h"
#include "bus/controll.h"
#include "bus/data.h"
#include "cpu/cpu.h"
#include "graphics/ppu.h"
#include "memory/memory.h"

#include "graphics/screen.h"


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
    Memory::Memory memory(&DBus, &ABus, &CBus); 


    memory.loadb(pROMData, (size_t)size);
    DBus.clock(); ABus.clock(); CBus.clock();
    
    Graphics::PPU ppu(memory.getVRAM(), memory.getOAM());

    long long clock = 0;
    while (true) {
        std::cout << std::to_string(clock++) << std::endl;
        cpu.clock();
        memory.clock();

        ppu.clock();

        DBus.clock(); ABus.clock(); CBus.clock();
    }
    SDL_Quit();
    delete[] pROMData;
}