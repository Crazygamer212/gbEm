
#include "cpu/cpu.h"


// clock cycle:
/*
    CPU
    MEM
    ...
    BUSSES
*/

int main() {
    Cpu::Cpu cpu;
    while (true) {
        cpu.clock();
    }
}