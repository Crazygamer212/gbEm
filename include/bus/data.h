#pragma once

#include <sys/types.h>
#include <stdexcept>



namespace Cpu {
    namespace Bus {
        struct Data { // 8-bit bi
        private:
            u_int8_t data{0};
            bool written_this_clock{false};
        public:
            void clock();
            u_int8_t read();
            void write(u_int8_t data);
        };
    }
}