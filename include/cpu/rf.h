#pragma once

#include <sys/types.h>

namespace Cpu {
    struct RegisterFile {
        u_int16_t PC;
        u_int16_t SP;
        union {
            u_int16_t AF;
            struct {
                u_int8_t A;
                u_int8_t F;
            };
        };
        union {
            u_int16_t BC;
            struct {
                u_int8_t B;
                u_int8_t C;
            };
        };
        union {
            u_int16_t DE;
            struct {
                u_int8_t D;
                u_int8_t E;
            };
        };
        union {
            u_int16_t HL;
            struct {
                u_int8_t H;
                u_int8_t L;
            };
        };
        u_int8_t IR;
        u_int8_t IE;

        void clock(); // NOT USED
    };
}
