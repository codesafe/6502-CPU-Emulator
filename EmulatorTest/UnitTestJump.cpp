#include "UnitTestMain.h"


TEST(TEST_JUMP, INST_JMP_ABS)
{
	cpu.Reset();

	mem[0xFFFC] = JMP_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	CPU CPUCopy = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
	EXPECT_EQ(cpu.PC, 0x8000);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_JUMP, INST_JMP_IND)
{
	cpu.Reset();

	mem[0xFFFC] = JMP_IND;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 0x00;
	mem[0x8001] = 0x90;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
	EXPECT_EQ(cpu.SP, CPUCopy.SP);
	EXPECT_EQ(cpu.PC, 0x9000);

	CheckCPUFlagModified(cpu, CPUCopy);
}


// 이건 ABS만 있다
TEST(TEST_JUMP, INST_JSR)
{
	cpu.Reset();
	cpu.SetPCAddress(0xFF00);

	mem[0xFF00] = JSR;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;

	mem[0x8000] = RTS;

	CPU CPUCopy = cpu;
	const int _cycle = 6 + 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PS, CPUCopy.PS);
	EXPECT_EQ(cpu.PC, 0xFF03);

	//CheckCPUFlagModified(cpu, CPUCopy);
}
