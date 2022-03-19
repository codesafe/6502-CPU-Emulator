#include "UnitTestMain.h"

TEST(TEST_SYSTEM, INST_BRK)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = BRK;
	// interrupt vector
	mem[0xFFFE] = 0x00;
	mem[0xFFFF] = 0x80;

	CPU CPUCopy = cpu;
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PC, 0x8000);

	// Little endian은 이렇게 Push
	EXPECT_EQ(mem[(0x100 | CPUCopy.SP) - 0], 0xFF);
	EXPECT_EQ(mem[(0x100 | CPUCopy.SP) - 1], 0xFD);
	EXPECT_EQ(mem[(0x100 | CPUCopy.SP) - 2], CPUCopy.PS);
}

TEST(TEST_SYSTEM, INST_RTI)
{
	cpu.Reset();

	mem[0xFFFC] = BRK;
	mem[0xFFFE] = 0x00;
	mem[0xFFFF] = 0x80;
	mem[0x8000] = RTI;

	CPU CPUCopy = cpu;
	const int _cycle = 7 + 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(CPUCopy.SP, cpu.SP);
	EXPECT_EQ(0xFFFD, cpu.PC);
	EXPECT_EQ(CPUCopy.PS, cpu.PS);
}
