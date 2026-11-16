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
                u_int8_t F;
                u_int8_t A;
            };
        };
        union {
            u_int16_t BC;
            struct {
                u_int8_t C;
                u_int8_t B;
            };
        };
        union {
            u_int16_t DE;
            struct {
                u_int8_t E;
                u_int8_t D;
            };
        };
        union {
            u_int16_t HL;
            struct {
                u_int8_t L;
                u_int8_t H;
            };
        };
        u_int8_t IR;
        u_int8_t CBIR;
        u_int8_t IE;

        u_int8_t Temp; //TEMP
        u_int16_t Temp16;
        

        void clock(); // NOT USED

        u_int8_t* addr8bmap(int num);
        u_int16_t* addr16bmap(int num);

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
