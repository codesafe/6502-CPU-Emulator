#include "UnitTestMain.h"


TEST(TEST_JMP, ABS)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x2F);
	mem[0xFFFC] = JMP_ABS;
	mem[0xFFFD] = 0x80;

	mem[0x0080] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 3;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x0080], 0x2F);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_JMP, IND)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = JMP_IND;
	mem[0xFFFD] = 0x80;

	mem[0x008F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x008F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}


// 이건 ABS만 있다
TEST(TEST_JSR, ABS)
{
	cpu.Reset();

	mem[0xFFFC] = JSR;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = RTS;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x008F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_RTS, IMP)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = RTS;
	mem[0xFFFD] = 0x80;

	mem[0x008F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x008F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}


