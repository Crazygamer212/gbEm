#include <gtest/gtest.h>
#include "cpu/cpu.h"

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(CPU, Creation) {
    Cpu::Cpu* pcpu = new Cpu::Cpu();
    delete pcpu;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}