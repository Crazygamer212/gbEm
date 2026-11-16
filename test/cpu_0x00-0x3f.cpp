#include <gtest/gtest.h>
#include <sys/types.h>

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


TEST_F(CPUTest, LD_i2r) {
    // M1
    DBus.write(0x06);// LD B,n
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock();
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(73);

    DBus.clock(); ABus.clock(); CBus.clock();
    // M2
    CU.clock();

    EXPECT_EQ(ABus.read(), 0x02);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.B, 73);
    DBus.clock(); ABus.clock(); CBus.clock();
    // M3
    // ...
}

TEST_F(CPUTest, LD_i2HL) {

    RF.HL = 0x1234;

    DBus.write(0x36);// LD (HL), n
    DBus.clock(); ABus.clock(); CBus.clock();
    // M2
    CU.clock(); 
    EXPECT_EQ(ABus.read(), 0x01); // = PC
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(87);

    DBus.clock(); ABus.clock(); CBus.clock();
    // M3
    CU.clock(); 

    EXPECT_EQ(ABus.read(), 0x1234); // = HL
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 87);

    DBus.clock(); ABus.clock(); CBus.clock();
    // M4
    // ...
}





TEST_F(CPUTest, LD_R2a) {

    RF.BC = 0x1112;

    DBus.write(0x0A);// LD A, (BC)
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x1112);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(60);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.A, 60);
}



TEST_F(CPUTest, LD_a2R) {

    RF.DE = 0x1312;
    RF.A  = 78;

    DBus.write(0x12);// LD (DE), A
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x1312);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 78);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
}




TEST_F(CPUTest, LD_HLdec2a) {

    RF.HL = 0x99f1;

    DBus.write(0x3A);// LD A, (HL-)
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x99f1);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(79);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.A, 79);
    EXPECT_EQ(RF.HL, 0x99f0);
}



TEST_F(CPUTest, LD_a2HLdec) {

    RF.HL = 0x4345;
    RF.A  = 44;

    DBus.write(0x32);// LD (HL-), A
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x4345);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 44);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.A, 44);
    EXPECT_EQ(RF.HL, 0x4344);
}






TEST_F(CPUTest, LD_HLinc2a) {

    RF.HL = 0x99f1;

    DBus.write(0x2A);// LD A, (HL+)
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x99f1);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(79);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.A, 79);
    EXPECT_EQ(RF.HL, 0x99f2);
}



TEST_F(CPUTest, LD_a2HLinc) {

    RF.HL = 0x4345;
    RF.A  = 44;

    DBus.write(0x22);// LD (HL+), A
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x4345);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 44);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3

    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.A, 44);
    EXPECT_EQ(RF.HL, 0x4346);
}




TEST_F(CPUTest, LD_I2R) {
    //little endian
    DBus.write(0x01);// LD BC, nn
    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x33); // fetch low byte

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x02);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x44); // fetch high byte

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4
    EXPECT_EQ(ABus.read(), 0x03);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::read);
    EXPECT_EQ(RF.BC, 0x4433);
}



TEST_F(CPUTest, LD_I2SP) {
    RF.SP = 0x0080;
    DBus.write(0x31);// LD SP, n16

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0xcc);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x02);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0xdd);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(RF.SP, 0xddcc);
    EXPECT_EQ(ABus.read(), 0x03);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}

TEST_F(CPUTest, LD_SP2I) {
    RF.SP = 0x0080;
    DBus.write(0x08);// LD (nn), SP

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0xcc);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x02);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0xdd);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4
    EXPECT_EQ(ABus.read(), 0xddcc);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 0x80);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M5
    EXPECT_EQ(ABus.read(), 0xccde);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 0x00);


    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M1/6
    EXPECT_EQ(ABus.read(), 0x03);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}




TEST_F(CPUTest, INC_r) {
    RF.B = 0xac;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x04);// INC B

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.B, 0xad);
    EXPECT_EQ(RF.getZ(), 0);
    EXPECT_EQ(RF.getC(), 0);
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getN(), 0);
}

TEST_F(CPUTest, INC_r_overflow) {
    RF.B = 0xff;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x04);// INC B

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.B, 0x00);
    EXPECT_EQ(RF.getZ(), 1);
    EXPECT_EQ(RF.getC(), 1);
    EXPECT_EQ(RF.getH(), 1);
    EXPECT_EQ(RF.getN(), 0);
}


TEST_F(CPUTest, INC_HL) {
    RF.HL = 0xff70;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x34);// INC (HL)

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0xff70);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x68);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0xff70);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 0x69);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.getZ(), 0);
    EXPECT_EQ(RF.getC(), 0);
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getN(), 0);
}





TEST_F(CPUTest, DEC_r) {
    RF.B = 0xb5;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x05);// DEC B

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.B, 0xb4);
    EXPECT_EQ(RF.getZ(), 0);
    EXPECT_EQ(RF.getC(), 0);
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getN(), 1);
}

TEST_F(CPUTest, DEC_r_overflow) {
    RF.B = 0x00;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(0);
    DBus.write(0x05);// Dec B

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.B, 0xff);
    EXPECT_EQ(RF.getZ(), 0);
    EXPECT_EQ(RF.getC(), 1);
    EXPECT_EQ(RF.getH(), 1);
    EXPECT_EQ(RF.getN(), 1);
}


TEST_F(CPUTest, DEC_HL) {
    RF.HL = 0xff70;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(0);
    DBus.write(0x35);// DEC (HL)

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0xff70);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(0x68);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0xff70);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::write);
    EXPECT_EQ(DBus.read(), 0x67);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.getZ(), 0);
    EXPECT_EQ(RF.getC(), 0);
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getN(), 1);
}



TEST_F(CPUTest, CCF) {
    RF.B = 0x00;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(1);
    RF.setN(1);
    DBus.write(0x3f);// CCF

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.getZ(), 1); // no change
    EXPECT_EQ(RF.getC(), 1); // gets flipped
    EXPECT_EQ(RF.getH(), 0); // reset
    EXPECT_EQ(RF.getN(), 0); // reset
}


TEST_F(CPUTest, SCF) {
    RF.B = 0x00;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(1);
    RF.setN(1);
    DBus.write(0x37);// SCF

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.getZ(), 1); // no change
    EXPECT_EQ(RF.getC(), 1); // set
    EXPECT_EQ(RF.getH(), 0); // reset
    EXPECT_EQ(RF.getN(), 0); // reset
}

//TODO in the not so complete technical reference
TEST_F(CPUTest, DAA) {
    RF.B = 0x00;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(1);
    RF.setN(1);
    DBus.write(0x27);// CSF

    EXPECT_TRUE(false);
}

TEST_F(CPUTest, CPL) {
    RF.A = 0xaf;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(1);
    RF.setN(1);
    DBus.write(0x2F);// CPL

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.A, 0x50); // ~A
    EXPECT_EQ(RF.getN(), 1);
    EXPECT_EQ(RF.getH(), 1);
}



TEST_F(CPUTest, INC_R) {
    RF.BC = 0x02ac;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x03);// INC BC

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.BC, 0x02ad);
    //EXPECT_EQ(RF.getZ(), 0);
    //EXPECT_EQ(RF.getC(), 0);
    //EXPECT_EQ(RF.getH(), 0);
    //EXPECT_EQ(RF.getN(), 0);
    //TODO gbctr doesn't mention flags but i susspect it should?
}


TEST_F(CPUTest, DEC_R) {
    RF.BC = 0x02ac;
    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x0B);// DEC BC

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(ABus.read(), 0x01);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

    EXPECT_EQ(RF.BC, 0x02ab);
    //EXPECT_EQ(RF.getZ(), 0);
    //EXPECT_EQ(RF.getC(), 0);
    //EXPECT_EQ(RF.getH(), 0);
    //EXPECT_EQ(RF.getN(), 0);
    //TODO gbctr doesn't mention flags but i susspect it should? unless IDU?
}


TEST_F(CPUTest, ADD_HL_R) {
    RF.BC = 0x3201;
    RF.H = 0x29;
    RF.L = 0xff;

    RF.setZ(1);
    RF.setC(0);
    RF.setH(0);
    RF.setN(1);
    DBus.write(0x09);// ADD HL, BC

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 1);
    EXPECT_EQ(RF.getH(), 1);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 0);
    EXPECT_EQ(RF.getH(), 0);

    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x01);

    EXPECT_EQ(RF.BC, 0x5c00);
}



TEST_F(CPUTest, RLCA) {
    RF.A = 0b10010101;

    RF.setZ(1);
    RF.setC(0);
    RF.setN(1);
    RF.setH(1);
    DBus.write(0x07);// RLCA

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 1); // rolled from msb
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getZ(), 0);

    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x01);

    EXPECT_EQ(RF.A, 0b00100101);
}

TEST_F(CPUTest, RRCA) {
    RF.A = 0b10010101;

    RF.setZ(1);
    RF.setC(0);
    RF.setN(1);
    RF.setH(1);
    DBus.write(0x07);// RLCA

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 1); // rolled from lsb
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getZ(), 0);

    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x01);

    EXPECT_EQ(RF.A, 0b11001010);
}


TEST_F(CPUTest, RLA) {
    RF.A = 0b10010101;

    RF.setZ(1);
    RF.setC(0);
    RF.setN(1);
    RF.setH(1);
    DBus.write(0x17);// RLA

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 1); // rolled from msb
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getZ(), 0);

    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x01);

    EXPECT_EQ(RF.A, 0b00101010); // 0 at end came from carry flag
}

TEST_F(CPUTest, RRA) {
    RF.A = 0b10010101;

    RF.setZ(1);
    RF.setC(0);
    RF.setN(1);
    RF.setH(1);
    DBus.write(0x1F );// RRA

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(RF.getN(), 0);
    EXPECT_EQ(RF.getC(), 1); // rolled from lsb
    EXPECT_EQ(RF.getH(), 0);
    EXPECT_EQ(RF.getZ(), 0);

    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    EXPECT_EQ(ABus.read(), 0x01);

    EXPECT_EQ(RF.A, 0b01001010); // 0 at end came from carry flag
}




TEST_F(CPUTest, JR_e_negative) {
    RF.PC = 0x101;
    DBus.write(0x18 );// JR -0x31    assumes JR starts at 0x100

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write(static_cast<u_int8_t>((int8_t)-0x31));

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(ABus.read(), 0xD1);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);

}

TEST_F(CPUTest, JR_e_possitive) {
    RF.PC = 0x101;
    DBus.write(0x18 );// JR 0x32    assumes JR starts at 0x100

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2     PC = 0x102 here?
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write((u_int8_t)0x32);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(ABus.read(), 0x134);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}


TEST_F(CPUTest, JR_cc_e__failed) {
    RF.PC = 0x101;
    RF.setZ(1); // if Z = 0 then branch
    DBus.write(0x20 );// JR 0x20    assumes JR starts at 0x100

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write((u_int8_t)0x32);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3/1
    EXPECT_EQ(ABus.read(), 0x102);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}

TEST_F(CPUTest, JR_cc_e__success) {
    RF.PC = 0x101;
    RF.setZ(0);
    DBus.write(0x20 );// JR 0x20    assumes JR starts at 0x100

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M2
    EXPECT_EQ(ABus.read(), 0x101);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
    DBus.write((int8_t)-0x32);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M3
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::idle);

    DBus.clock(); ABus.clock(); CBus.clock();
    CU.clock(); //M4/1
    EXPECT_EQ(ABus.read(), 0xD0);
    EXPECT_EQ(CBus.getState(), Cpu::Bus::State::read);
}



