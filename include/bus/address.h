#pragma once

#include <sys/types.h>
#include <stdexcept>



namespace Cpu {
    namespace Bus {
        class Addr { // 16-bit address - piped to idu? + out
            u_int16_t data{0};
            bool written_this_clock{false};
        public:
            void clock();
            u_int16_t read();
            void write(u_int16_t data);
        };
    }
}