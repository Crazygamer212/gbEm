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


TEST(CPU, ADD_A2r_oveflow_zero) {
    Cpu::RegisterFile *pRF = new Cpu::RegisterFile();
    Cpu::Bus::Data *pDBus = new Cpu::Bus::Data();
    Cpu::Bus::Addr *pABus = new Cpu::Bus::Addr();
    Cpu::Bus::Ctrl *pCBus = new Cpu::Bus::Ctrl();

    Cpu::ALU *pALU = new Cpu::ALU(pDBus, pRF);
    Cpu::IDU *pIDU = new Cpu::IDU(pRF, pABus, pCBus);
    Cpu::ControllUnit *pCU  = new Cpu::ControllUnit(pRF, pABus, pDBus, pCBus, pALU, pIDU);

    pRF->B=0x05;
    pRF->C=0x0A;
    pRF->D=0x23;
    pRF->E=0x68;

    pRF->HL=0x3137;
    pRF->A=0x98;

    pRF->SP=0x1000;
    pRF->PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(pABus->read(), 0x0000);
    pDBus->write(0x83);// ADD E[, A] -- A is implied 
    pDBus->clock(); // should overflow to 0x00
    pABus->clock();
    pCBus->clock();
    pCU->clock();

    EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    EXPECT_EQ(pABus->read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(pRF->F & 0b00001111, 0b00001101);
    EXPECT_EQ(pRF->A, 0x00);


    delete(pCU);
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
    delete(pCBus);
    delete(pRF);
}

TEST(CPU, ADD_A2r_normal) {
    Cpu::RegisterFile *pRF = new Cpu::RegisterFile();
    Cpu::Bus::Data *pDBus = new Cpu::Bus::Data();
    Cpu::Bus::Addr *pABus = new Cpu::Bus::Addr();
    Cpu::Bus::Ctrl *pCBus = new Cpu::Bus::Ctrl();

    Cpu::ALU *pALU = new Cpu::ALU(pDBus, pRF);
    Cpu::IDU *pIDU = new Cpu::IDU(pRF, pABus, pCBus);
    Cpu::ControllUnit *pCU  = new Cpu::ControllUnit(pRF, pABus, pDBus, pCBus, pALU, pIDU);

    pRF->B=0x21;
    pRF->C=0x2A;
    pRF->D=0x23;
    pRF->E=0x69;

    pRF->HL=0x3177;
    pRF->A=0x3A;

    pRF->SP=0x1000;
    pRF->PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(pABus->read(), 0x0000);
    pDBus->write(0x80);// ADD B[, A] -- A is implied 
    pDBus->clock(); // should overflow to 0x00
    pABus->clock();
    pCBus->clock();
    pCU->clock();

    EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    EXPECT_EQ(pABus->read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(pRF->F & 0b00001111, 0b00000000);
    EXPECT_EQ(pRF->A, 0x5B);


    delete(pCU);
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
    delete(pCBus);
    delete(pRF);
}





TEST(CPU, ADC_A2r) {
    Cpu::RegisterFile *pRF = new Cpu::RegisterFile();
    Cpu::Bus::Data *pDBus = new Cpu::Bus::Data();
    Cpu::Bus::Addr *pABus = new Cpu::Bus::Addr();
    Cpu::Bus::Ctrl *pCBus = new Cpu::Bus::Ctrl();

    Cpu::ALU *pALU = new Cpu::ALU(pDBus, pRF);
    Cpu::IDU *pIDU = new Cpu::IDU(pRF, pABus, pCBus);
    Cpu::ControllUnit *pCU  = new Cpu::ControllUnit(pRF, pABus, pDBus, pCBus, pALU, pIDU);

    pRF->B=0x65;
    pRF->C=0x0A;
    pRF->D=0x23;
    pRF->E=0x68;

    pRF->HL=0x3137;
    pRF->A=0x3A;

    pRF->SP=0x1000;
    pRF->PC=1;

    //------SETUP-DONE-------

    ///EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    //EXPECT_EQ(pABus->read(), 0x0000);
    pDBus->write(0x80);// ADD B[, A] -- A is implied 
    pDBus->clock(); // should overflow to 0x00
    pABus->clock();
    pCBus->clock();
    pCU->clock();

    EXPECT_EQ(pCBus->getState(),Cpu::Bus::State::read);
    EXPECT_EQ(pABus->read(), 0x0001);

                            // 0b____chnz   -- half carry ( 0x0f+0x01 would trigger due to carry at bit 4)
    EXPECT_EQ(pRF->F & 0b00001111, 0b00000000);
    EXPECT_EQ(pRF->A, 0x5B);


    delete(pCU);
    delete(pALU);
    delete(pIDU);

    delete(pABus);
    delete(pDBus);
    delete(pCBus);
    delete(pRF);
}

