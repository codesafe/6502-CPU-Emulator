#include "UnitTestMain.h"

TEST(TEST_BRANCH, INST_BEQ)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.Z = true;
	
	cpu.SetPCAddress(0xFEFD);

	mem[0xFEFD] = BEQ;
	mem[0xFEFE] = 0x0E;

	mem[0xFEFF + 0x0E] = INY;
	mem[0xFEFF + 0x0F] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 4 +2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}


TEST(TEST_BRANCH, INST_BEQ_NEGATIVE)
{
	cpu.Reset();

	cpu.Flag.Z = true;
	cpu.SetPCAddress(0xFFCC);

	mem[0xFFCC] = BEQ;
	mem[0xFFCD] = -2;

	CPU CPUCopy = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PC, 0xFFCC);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);

}

TEST(TEST_BRANCH, INST_BNE)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.Z = false;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BNE;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}


TEST(TEST_BRANCH, INST_BCS)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.C = true;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BCS;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}

TEST(TEST_BRANCH, INST_BCC)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.C = false;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BCC;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}

TEST(TEST_BRANCH, INST_BMI)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.N = true;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BMI;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}

TEST(TEST_BRANCH, INST_BPL)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.N = false;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BPL;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}


TEST(TEST_BRANCH, INST_BVC)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.V = false;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BVC;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}

TEST(TEST_BRANCH, INST_BVS)
{
	cpu.Reset();

	cpu.X = 0x32;
	cpu.Y = 0x42;
	cpu.Flag.V = true;

	cpu.SetPCAddress(0xFEF0);

	mem[0xFEF0] = BVS;
	mem[0xFEF1] = 0x03;

	mem[0xFEF2 + 0x03] = INY;
	mem[0xFEF2 + 0x04] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 3 + 2 + 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x43);
	EXPECT_EQ(cpu.X, 0x33);
}

