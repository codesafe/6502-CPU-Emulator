#include "UnitTestMain.h"

TEST(TEST_TRANSFER_REGISTER, INST_TAX)
{
	cpu.Reset();
	cpu.A = 0x77;
	cpu.X = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	// Transfer Accumulator to X
	mem[0xFFFC] = TAX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, cpu.X);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}


TEST(TEST_TRANSFER_REGISTER, INST_TAY)
{
	cpu.Reset();
	cpu.A = 0x77;
	cpu.Y = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = TAY;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, cpu.Y);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST(TEST_TRANSFER_REGISTER, INST_TXA)
{
	cpu.Reset();
	//cpu.X = 0b10001000; // 0x88;
	cpu.X = 0;
	cpu.A = 0x77;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = TXA;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, cpu.X);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST(TEST_TRANSFER_REGISTER, INST_TYA)
{
	cpu.Reset();
	cpu.Y = 0b10010111;
	cpu.A = 0x77;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = TYA;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b10010111);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}



