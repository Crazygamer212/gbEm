#pragma once

#include <sys/types.h>
#include <stdexcept>



namespace Cpu {
    namespace Bus {
        class Data { // 8-bit bi
            u_int8_t data{0};
            bool written_this_clock{false};
        public:
            void clock();
            u_int8_t read();
            void write(u_int8_t data);
        };
    }
}