#pragma once

#include <sys/types.h>



namespace Cpu {
    namespace Bus {
        struct Addr { // 16-bit address - piped to idu? + out
            u_int16_t data = 0;
            void clock();
        };
    }
}