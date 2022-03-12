#include "UnitTestMain.h"

TEST(TEST_INCREASE_DECREASE, INST_INX)
{
	cpu.Reset();
	cpu.X = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.X, 0x89);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST(TEST_INCREASE_DECREASE, INST_INY)
{
	cpu.Reset();
	cpu.Y = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = INY;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x89);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST(TEST_INCREASE_DECREASE, INST_DEX)
{
	cpu.Reset();
	cpu.X = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = DEX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.X, 0x87);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST(TEST_INCREASE_DECREASE, INST_DEY)
{
	cpu.Reset();
	cpu.Y = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = DEY;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.Y, 0x87);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TEST_INCREASE_DECREASE, INST_DEC_ZP)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;


	mem[0xFFFC] = DEC_ZP;
	mem[0xFFFD] = 0x80;

	mem[0x0080] = 0x77;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0080], 0x76);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}


TEST(TEST_INCREASE_DECREASE, INST_DEC_ZPX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;

	cpu.X = 0x10;

	mem[0xFFFC] = DEC_ZPX;
	mem[0xFFFD] = 0x80;

	mem[0x0090] = 0x77;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0090], 0x76);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}



TEST(TEST_INCREASE_DECREASE, INST_DEC_ABS)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = DEC_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;
	
	mem[0x8000] = 0x88;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 0x87);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}



TEST(TEST_INCREASE_DECREASE, INST_DEC_ABSX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;
	cpu.X = 0x10;

	mem[0xFFFC] = DEC_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;
	
	mem[0x8010] = 0x88;

	CPU CPUCopy = cpu;
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8010], 0x87);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TEST_INCREASE_DECREASE, INST_INC_ZP)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;


	mem[0xFFFC] = INC_ZP;
	mem[0xFFFD] = 0x80;

	mem[0x0080] = 0x77;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0080], 0x78);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}


TEST(TEST_INCREASE_DECREASE, INST_INC_ZPX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = true;

	cpu.X = 0x10;

	mem[0xFFFC] = INC_ZPX;
	mem[0xFFFD] = 0x80;

	mem[0x0090] = 0x77;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0090], 0x78);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}



TEST(TEST_INCREASE_DECREASE, INST_INC_ABS)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = INC_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;
	
	mem[0x8000] = 0x88;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 0x89);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}



TEST(TEST_INCREASE_DECREASE, INST_INC_ABSX)
{
	cpu.Reset();
	cpu.Flag.Z = cpu.Flag.N = false;
	cpu.X = 0x10;

	mem[0xFFFC] = INC_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;
	
	mem[0x8010] = 0x88;

	CPU CPUCopy = cpu;
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8010], 0x89);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}
