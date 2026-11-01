#pragma once



namespace Cpu {
    namespace Bus {
        struct Data { // 8-bit bi
            u_int8_t data = 0;
            void clock();
        };
        struct Addr { // 16-bit address - piped to idu? + out
            u_int16_t data = 0;
            void clock();
        };
    }


    class Cpu {
        RegisterFile* pRF;
        Bus::Data* pDBus;
        Bus::Addr* pABus;

        ALU* pALU;
        IDU* pIDU;

        int timer;

    public:
        Cpu(/*SoC DBus, SoC ABus*/);
        ~Cpu();
        
        void clock();
        void iclock();
    };


}