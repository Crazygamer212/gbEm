#pragma once

#include <sys/types.h>



namespace Cpu {
    namespace Bus {
        struct Data { // 8-bit bi
            u_int8_t data = 0;
            void clock();
        };
    }
}