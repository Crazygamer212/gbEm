#pragma once

#include <cstdlib>
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

        u_int8_t Temp; //TEMP
        

        void clock(); // NOT USED

        u_int8_t* addr8bmap(int num);

        void setZ(bool status);
        void setN(bool status);
        void setH(bool status);
        void setC(bool status);
        
        u_int8_t getZ();
        u_int8_t getN();
        u_int8_t getH();
        u_int8_t getC();
    };
}
