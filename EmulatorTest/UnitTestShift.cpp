#include "UnitTestMain.h"

TEST(TEST_SHIFT, INST_ASL)
{
	cpu.Reset();

	cpu.A = 0b00110001;		// B1
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ASL_NEGATIVE)
{
	cpu.Reset();

	cpu.A = 0b10110001;		// B1
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ASL_ZP)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL_ZP;
	mem[0xFFFD] = 0x42;

	mem[0x0042] = 0b10110001;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ASL_ZPX)
{
	cpu.Reset();

	cpu.X = 0x5;
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL_ZPX;
	mem[0xFFFD] = 0x42;

	mem[0x0047] = 0b10110001;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ASL_ABS)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 0b10110001;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ASL_ABSX)
{
	cpu.Reset();

	cpu.X = 0x04;
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ASL_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8004] = 0b10110001;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8004], 0b01100010);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TEST_SHIFT, INST_LSR)
{
	cpu.Reset();

	cpu.A = 1;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = LSR;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_LSR_2)
{
	cpu.Reset();

	cpu.A = 8;
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LSR;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 4);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_LSR_ZP)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LSR_ZP;
	mem[0xFFFD] = 0x42;

	mem[0x0042] = 1;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_LSR_ZPX)
{
	cpu.Reset();

	cpu.X = 0x5;
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LSR_ZPX;
	mem[0xFFFD] = 0x42;

	mem[0x0047] = 1;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_LSR_ABS)
{
	cpu.Reset();

	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LSR_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 1;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_LSR_ABSX)
{
	cpu.Reset();

	cpu.X = 0x04;
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LSR_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8004] = 1;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8004], 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define ROL			0x2A
#define ROL_ZP		0x26
#define ROL_ZPX		0x36
#define ROL_ABS		0x2E
#define ROL_ABSX	0x3E

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define ROR			0x6A
#define ROR_ZP		0x66
#define ROR_ZPX		0x76
#define ROR_ABS		0x6E
#define ROR_ABSX	0x7E
