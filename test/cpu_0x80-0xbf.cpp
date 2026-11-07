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
//TODO check tests + code -_-


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


TEST_F(CPUTest, ADD_A2r_oveflow_zero) {

    RF.B=0x05;
    RF.C=0x0A;
    RF.D=0x23;
    RF.E=0x68;

    RF.HL=0x3137;
    RF.A=0x98;

    RF.SP=0x1000;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x83);// ADD E[, A] -- A is implied 
    DBus.clock(); // should overflow to 0x00
    ABus.clock();
    CBus.clock();
    CU.clock();

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(RF.F & 0b00001111, 0b00001101);
    EXPECT_EQ(RF.A, 0x00);

}

TEST_F(CPUTest, ADD_A2r_normal) {

    RF.B=0x21;
    RF.C=0x2A;
    RF.D=0x23;
    RF.E=0x69;

    RF.HL=0x3177;
    RF.A=0x3A;

    RF.SP=0x1000;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x80);// ADD B[, A] -- A is implied 
    DBus.clock(); // should overflow to 0x00
    ABus.clock();
    CBus.clock();
    CU.clock();

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(RF.F & 0b00001111, 0b00000000);
    EXPECT_EQ(RF.A, 0x5b);


}


TEST_F(CPUTest, ADC_A2r) {

    RF.B=0x65;
    RF.C=0x0A;
    RF.D=0x50;
    RF.E=0x68;

    RF.HL=0x3137;
    RF.A=0x3f;

    RF.setC(1);

    RF.SP=0x1000;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x8A);// ADD D[, A] -- A is implied 
    DBus.clock(); // should overflow to 0x00
    ABus.clock();
    CBus.clock();
    CU.clock();

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(RF.F & 0b00001111, 0b00000100);
    EXPECT_EQ(RF.A, 0x90);


}



