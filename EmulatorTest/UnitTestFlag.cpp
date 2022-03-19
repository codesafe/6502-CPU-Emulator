#include "UnitTestMain.h"

TEST(TEST_FLAG, INST_CLC)
{
	cpu.Reset();

	cpu.Flag.C = true;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = CLC;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_FALSE(cpu.Flag.C);
}

TEST(TEST_FLAG, INST_CLD)
{
	cpu.Reset();

	cpu.Flag.D = true;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = CLD;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_FALSE(cpu.Flag.D);
}

TEST(TEST_FLAG, INST_CLI)
{
	cpu.Reset();

	cpu.Flag.I = true;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = CLI;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_FALSE(cpu.Flag.I);
}

TEST(TEST_FLAG, INST_CLV)
{
	cpu.Reset();

	cpu.Flag.I = true;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = CLV;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_FALSE(cpu.Flag.V);
}

	
TEST(TEST_FLAG, INST_SEC)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = SEC;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.C);
}


TEST(TEST_FLAG, INST_SED)
{
	cpu.Reset();

	cpu.Flag.D = false;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = SED;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.D);
}

TEST(TEST_FLAG, INST_SEI)
{
	cpu.Reset();

	cpu.Flag.I = false;
	cpu.SetPCAddress(0xFEFD);
	mem[0xFEFD] = SEI;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.I);
}

