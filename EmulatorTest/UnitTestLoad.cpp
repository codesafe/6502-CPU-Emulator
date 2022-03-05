#include "UnitTestMain.h"


TEST(TEST_LDA, IM)
{
	cpu.Reset();

	mem[0xFFFC] = LDA_IM;
	mem[0xFFFD] = 0x10;

	CPU CPUCopy = cpu;

	const int _cycle = 2;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x10);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LDA, ZP)
{
	cpu.Reset();

	mem[0xFFFC] = LDA_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 3;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 3);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDA, ZPX)
{
	cpu.Reset();

	cpu.X = 0xFF;
	mem[0xFFFC] = LDA_ZPX;
	mem[0xFFFD] = 0x80;
	mem[0x007F] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}



TEST(TEST_LDA, ABS)
{
	cpu.Reset();

	cpu.X = 5;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDA_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4480] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LDA, ABSX)
{
	cpu.Reset();

	cpu.X = 1;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDA_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4481] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}



TEST(TEST_LDA, ABSY)
{
	cpu.Reset();

	cpu.Y = 1;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDA_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4481] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDA, INDX)
{
	cpu.Reset();

	cpu.X = 4;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDA_INDX;
	mem[0xFFFD] = 0x02;
	
	mem[0x0006] = 0x00;	// 0x02 + 0x04
	mem[0x0007] = 0x80;

	mem[0x8000] = 0x37;

	CPU CPUCopy = cpu;

	constexpr int cycle = 6;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, cycle);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LDA, INDY)
{
	cpu.Reset();

	cpu.Y = 0x04;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDA_INDY;
	mem[0xFFFD] = 0x02;

	mem[0x0002] = 0x00;	// 0x02(fetch) + 0x04 (x Reg)
	mem[0x0003] = 0x80;

	mem[0x8004] = 0x37;

	CPU CPUCopy = cpu;

	constexpr int cycle = 5;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, cycle);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);

	CheckCPUFlagModified(cpu, CPUCopy);
}

///////////////////////////////////////////////////////////////////////////////////////


TEST(TEST_LDX, IM)
{
	cpu.Reset();

	mem[0xFFFC] = LDX_IM;
	mem[0xFFFD] = 0x10;

	CPU CPUCopy = cpu;

	const int _cycle = 2;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_X), 0x10);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_LDX, ZP)
{
	cpu.Reset();

	mem[0xFFFC] = LDX_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 3;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_X), 0x37);
	EXPECT_EQ(CyclesUsed, 3);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDX, ZPY)
{
	cpu.Reset();

	cpu.X = 0x5;
	mem[0xFFFC] = LDA_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_A), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDX, ABS)
{
	cpu.Reset();

	cpu.X = 5;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDX_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4480] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_X), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDX, ABSY)
{
	cpu.Reset();

	cpu.Y = 1;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDX_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4481] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_X), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

///////////////////////////////////////////////////////////////////////////////////////

TEST(TEST_LDY, IM)
{
	cpu.Reset();

	mem[0xFFFC] = LDY_IM;
	mem[0xFFFD] = 0x10;

	CPU CPUCopy = cpu;

	const int _cycle = 2;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_Y), 0x10);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}



TEST(TEST_LDY, ZP)
{
	cpu.Reset();

	mem[0xFFFC] = LDY_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 3;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_Y), 0x37);
	EXPECT_EQ(CyclesUsed, 3);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDY, ZPX)
{
	cpu.Reset();

	cpu.X = 0x5;
	mem[0xFFFC] = LDY_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_Y), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDY, ABS)
{
	cpu.Reset();

	cpu.X = 5;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDY_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4480] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_Y), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_LDY, ABSX)
{
	cpu.Reset();

	cpu.X = 1;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = LDY_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4481] = 0x37;

	CPU CPUCopy = cpu;

	int cycle = 4;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(cpu.GetRegister(REGISTER_Y), 0x37);
	EXPECT_EQ(CyclesUsed, 4);

	CheckZeroOrNegative();
	CheckCPUFlagModified(cpu, CPUCopy);
}
