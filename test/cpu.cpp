#include <gtest/gtest.h>

#include "cpu/cpu.h"

#include "bus/controll.h"
#include "cpu/cu.h"
#include "cpu/rf.h"
#include "cpu/alu.h"
#include "cpu/idu.h"

#include "bus/data.h"
#include "bus/address.h"


/*
--TEST-INF-----
I   I   
I   I   
I---I-----CPU---
I   I   
BUS CLK

*/



TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(CPU, Creation) {
    Cpu::Cpu* pcpu = new Cpu::Cpu();
    delete pcpu;
}




TEST(CPU, NOP) {
    Cpu::RegisterFile *pRF = new Cpu::RegisterFile();
    Cpu::Bus::Data *pDBus = new Cpu::Bus::Data();
    Cpu::Bus::Addr *pABus = new Cpu::Bus::Addr();
    Cpu::Bus::Ctrl *pCBus = new Cpu::Bus::Ctrl();

    Cpu::ALU *pALU = new Cpu::ALU(pDBus, pRF);
    Cpu::IDU *pIDU = new Cpu::IDU(pRF, pABus, pCBus);
    Cpu::ControllUnit *pCU  = new Cpu::ControllUnit(pRF, pABus, pDBus, pCBus, pALU, pIDU);

    pRF->B=5;
    pRF->C=11;
    pRF->A=6;
    pRF->D=9;
    pRF->HL=0x0130;
    pRF->PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(pABus->read(), 0x0000);
    pDBus->write(0x00);// NOP
    pCU->clock();
    pDBus->clock(); pABus->clock(); pCBus->clock();
    pDBus->write(0x00);// return next instruction

    EXPECT_EQ(pRF->PC, 0x02);
    EXPECT_EQ(pRF->HL, 0x0130);

    pDBus->clock(); pABus->clock(); pCBus->clock();

    EXPECT_EQ(pRF->D, 0x09);

    EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    EXPECT_EQ(pABus->read(), 0x0001);


    delete(pCU);
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
    delete(pCBus);
    delete(pRF);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}