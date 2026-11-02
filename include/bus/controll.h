#pragma once



namespace Cpu {
    namespace Bus {

        enum State {
            idle = 0,
            read = 3,
            write = 2,
        };

        struct Ctrl {
            Cpu::Bus::State state{State::idle};
            bool written_this_clock{false};

            void clock(); // debug; does nada
            void setState(State state); // should not be writen to by memory
            State getState(); // should be read by memory only
        };
    }
}