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
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8004], 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


TEST(TEST_SHIFT, INST_ROL)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROL;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 1);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ROL2)
{
	cpu.Reset();

	cpu.A = 0b11010011;
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;

	mem[0xFFFC] = ROL;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b10100110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}




TEST(TEST_SHIFT, INST_ROL_ZP)
{
	cpu.Reset();

	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROL_ZP;
	mem[0xFFFD] = 0x42;

	mem[0x0042] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 1);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ROL_ZPX)
{
	cpu.Reset();

	cpu.X = 0x5;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROL_ZPX;
	mem[0xFFFD] = 0x42;

	mem[0x0047] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 1);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ROL_ABS)
{
	cpu.Reset();

	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROL_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 1);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ROL_ABSX)
{
	cpu.Reset();

	cpu.X = 0x04;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROL_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8004] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8004], 1);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////


TEST(TEST_SHIFT, INST_ROR)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = ROR;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b10000000);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ROR2)
{
	cpu.Reset();

	cpu.A = 0b01101101;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = ROR;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0b10110110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

TEST(TEST_SHIFT, INST_ROR_ZP)
{
	cpu.Reset();

	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = ROR_ZP;
	mem[0xFFFD] = 0x42;

	mem[0x0042] = 0b01101101;

	CPU CPUCopy = cpu;
	const int _cycle = 5;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0b10110110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ROR_ZPX)
{
	cpu.Reset();

	cpu.X = 0x5;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = ROR_ZPX;
	mem[0xFFFD] = 0x42;

	mem[0x0047] = 0b01101101;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x0042], 0b10110110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ROR_ABS)
{
	cpu.Reset();

	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = false;

	mem[0xFFFC] = ROR_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 0b01101101;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8000], 0b10110110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}


TEST(TEST_SHIFT, INST_ROR_ABSX)
{
	cpu.Reset();

	cpu.X = 0x04;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ROR_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8004] = 0b01101101;

	CPU CPUCopy = cpu;
	const int _cycle = 7;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(mem[0x8004], 0b10110110);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.V);
}

