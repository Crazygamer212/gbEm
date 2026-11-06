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



TEST_F(CPUTest, LD_r2r) {

    RF.B=5;
    RF.C=11;
    RF.A=6;
    RF.D=9;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x41);// LD B, C
    DBus.clock();
    ABus.clock();
    CBus.clock();
    CU.clock();
    EXPECT_EQ(RF.B, 11);

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);

    DBus.write(0x50); // LD D, B
    DBus.clock();
    ABus.clock();
    CBus.clock();
    CU.clock();

    EXPECT_EQ(RF.D, 11);
    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0002);


}



TEST_F(CPUTest, LD_memFr) {
    RF.B=5;
    RF.C=11;
    RF.A=6;
    RF.D=9;
    RF.PC=1;
    RF.HL=0x1337;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x71);// LD (HL), C
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock();

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::write);
    EXPECT_EQ(ABus.read(), 0x1337);
    EXPECT_EQ(DBus.read(), 11);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock();

    EXPECT_EQ(RF.D, 9);
    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);


}


TEST_F(CPUTest, LD_rFmem) {

    RF.B=5;
    RF.C=11;
    RF.A=6;
    RF.D=9;
    RF.HL=0x0130;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x56);// LD D, (HL)                M1
    CU.clock();
    DBus.clock(); ABus.clock(); CBus.clock();
    EXPECT_EQ(ABus.read(),0x0130);
    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    DBus.write(0x78);// return value in 0x0130

    
    CU.clock(); //                                     M2
    DBus.clock(); ABus.clock(); CBus.clock();

    EXPECT_EQ(RF.D, 0x78);

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);

}

TEST_F(CPUTest, HALT__LD_memFmem) {

    RF.B=5;
    RF.C=11;
    RF.A=6;
    RF.D=9;
    RF.HL=0x0130;
    RF.PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(ABus.read(), 0x0000);
    DBus.write(0x76);// LD (HL), (HL)                M1
    CU.clock();
    DBus.clock(); ABus.clock(); CBus.clock();
    EXPECT_EQ(ABus.read(),0x0130);
    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    DBus.write(0x78);// return value in 0x0130
    
    CU.clock(); //                                        M2
    DBus.clock(); ABus.clock(); CBus.clock();

    EXPECT_EQ(RF.D, 0x78);

    EXPECT_EQ(CBus.getState(),Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x0001);


}
