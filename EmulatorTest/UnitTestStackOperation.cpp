#include "UnitTestMain.h"

TEST(TEST_STACK, INST_TSX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;
	cpu.X = 0x08;
	cpu.SP = 0x01;

	mem[0xFFFC] = TSX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.Z);

}

TEST(TEST_STACK, INST_TXS)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;
	cpu.X = 0x08;
	cpu.SP = 0xA1;

	mem[0xFFFC] = TXS;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.X, 0x08);

}

TEST(TEST_STACK, INST_PHA)
{
	cpu.Reset();

	cpu.A = 0x48;
	mem[0xFFFC] = PHA;

	CPU clonecpu = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	//printf("SP : %x \n", cpu.GetStackAddress());
	EXPECT_EQ(cpu.A, mem[cpu.GetStackAddress()+1]);

	EXPECT_EQ(cpu.PS, clonecpu.PS);
	EXPECT_EQ(cpu.SP, 0xFE);
}


TEST(TEST_STACK, INST_PLA)
{
	cpu.Reset();

	cpu.A = 0x48;

	int dummy = 0;
	cpu.PushStackByte(mem, 0x42, dummy);
	
// 	cpu.SP = 0xFE;
// 	mem[0x1FF] = 0x42;	// Stack 메모리 영역에 임의의 값

	mem[0xFFFC] = PLA;

	CPU clonecpu = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0x42);
	EXPECT_EQ(cpu.SP, 0xFF);
}


TEST(TEST_STACK, INST_PLP)
{
	cpu.Reset();

	int dummy = 0;
	cpu.PushStackByte(mem, 0x42, dummy);

	mem[0xFFFC] = PLP;

	CPU clonecpu = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.PS, 0x42);

}

