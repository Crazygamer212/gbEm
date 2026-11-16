#include <gtest/gtest.h>

#include "cpu/cpu.h"

#include "bus/controll.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"

//TODO sloppy tests - should define and check all RF.


class CPUTest : public ::testing::Test {
protected:
    Cpu::RegisterFile RF;
    Cpu::Bus::Data DBus;
    Cpu::Bus::Addr ABus;
    Cpu::Bus::Ctrl CBus;

    Cpu::ALU ALU{&DBus, &RF};
    Cpu::IDU IDU{&RF, &ABus, &CBus};

    Cpu::ControllUnit CU{
        &RF, &ABus, &DBus, &CBus, &ALU, &IDU
    };

    void SetUp() override {
        RF.B=0x05;
        RF.C=0x0A;
        RF.D=0x23;
        RF.E=0x68;

        RF.HL=0x3137;
        RF.A=0x98;

        RF.SP=0x1000;
        RF.PC=1;
    }

};



TEST_F(CPUTest, LD_I2A) {
    RF.PC = 0x101;
    RF.setZ(0);
    DBus.write(0xFA);// LD A, (nn)

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x33);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x102);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x44);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4
    EXPECT_EQ(ABus.read(), 0x4433);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x21);
    
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M5/1
    EXPECT_EQ(ABus.read(), 0x103);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(RF.A, 0x21);
}



TEST_F(CPUTest, LD_A2I) {
    RF.PC = 0x101;
    RF.A = 0x67;
    RF.setZ(0);
    DBus.write(0xEA);// LD (nn), A

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x33);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x102);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x44);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(ABus.read(), 0x4433);
    EXPECT_EQ(DBus.read(), 0x67);
    
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M5/1
    EXPECT_EQ(ABus.read(), 0x103);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}