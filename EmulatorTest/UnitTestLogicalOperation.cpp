#include "UnitTestMain.h"

TEST(TEST_LOGICAL, INST_AND_IM)
{
	cpu.Reset();
	cpu.A = 0b010101;

	mem[0xFFFC] = AND_IM;
	mem[0xFFFD] = 0b010001;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b010101 & 0b010001);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LOGICAL, INST_AND_ZP)
{
	cpu.Reset();
	cpu.A = 0xCC;

	mem[0xFFFC] = AND_ZP;
	mem[0xFFFD] = 0x42;

	mem[0x0042] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LOGICAL, INST_AND_ZPX)
{
	cpu.Reset();
	cpu.A = 0xCC;
	cpu.X = 0x05;

	mem[0xFFFC] = AND_ZPX;
	mem[0xFFFD] = 0x42;

	mem[0x0047] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LOGICAL, INST_AND_ABS)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.A = 0xCC;

	mem[0xFFFC] = AND_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480

	mem[0x4480] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LOGICAL, INST_AND_ABSX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.A = 0xCC;

	mem[0xFFFC] = AND_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480

	mem[0x4480] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LOGICAL, INST_AND_ABSY)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.A = 0xCC;
	cpu.Y = 1;

	mem[0xFFFC] = AND_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480

	mem[0x4481] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LOGICAL, INST_AND_INDX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.A = 0xCC;
	cpu.X = 0x04;

	mem[0xFFFC] = AND_INDX;
	mem[0xFFFD] = 0x02;
	mem[0x0006] = 0x00;	//0x2 + 0x4
	mem[0x0007] = 0x80;

	mem[0x8000] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LOGICAL, INST_AND_INDY)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.A = 0xCC;
	cpu.Y = 0x04;

	mem[0xFFFC] = AND_INDY;
	mem[0xFFFD] = 0x02;
	mem[0x0006] = 0x00;	//0x2 + 0x4
	mem[0x0007] = 0x80;

	mem[0x8000] = 0x37;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	const bool ExpectedNegative = ((0xCC & 0x37) & 0b10000000) > 0;

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0xCC & 0x37);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.Flag.N, ExpectedNegative);

	CheckCPUFlagModified(cpu, CPUCopy);
}

