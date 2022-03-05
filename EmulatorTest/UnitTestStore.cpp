#include "UnitTestMain.h"


TEST(TEST_STA, ZP)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x2F);
	mem[0xFFFC] = STA_ZP;
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

TEST(TEST_STA, ZPX)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = STA_ZPX;
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


TEST(TEST_STA, ABS)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x2F);

	mem[0xFFFC] = STA_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x800F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x8000], 0x2F);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_STA, ABSX)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = STA_ABSX;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	CPU CPUCopy = cpu;

	const int _cycle = 5;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x800F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}

TEST(TEST_STA, ABSY)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.Y = 0x0F;

	mem[0xFFFC] = STA_ABSY;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	CPU CPUCopy = cpu;

	const int _cycle = 5;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x800F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_STA, INDX)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = STA_INDX;
	mem[0xFFFD] = 0x20;

	mem[0x002F] = 0x00;
	mem[0x0030] = 0x80;
	mem[0x8000] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 6;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x8000], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}


TEST(TEST_STA, INDY)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_A, 0x42);
	cpu.Y = 0x0F;

	mem[0xFFFC] = STA_INDY;
	mem[0xFFFD] = 0x20;

	mem[0x0020] = 0x00;
	mem[0x0021] = 0x80;
	mem[0x8000] = 0x00;

	mem[0x8000 + 0x0F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 6;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x8000 + 0x0F], 0x42);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}

////////////////////////////////////////////////////////////////////////////////////

TEST(TEST_STX, ZP)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_X, 0x2F);
	mem[0xFFFC] = STX_ZP;
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

TEST(TEST_STX, ZPY)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_X, 0x42);
	cpu.Y = 0x0F;

	mem[0xFFFC] = STX_ZPY;
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

TEST(TEST_STX, ABS)
{
	cpu.Reset();
	cpu.SetRegister(REGISTER_X, 0x2F);

	mem[0xFFFC] = STX_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x800F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x8000], 0x2F);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}



////////////////////////////////////////////////////////////////////////////////////


TEST(TEST_STY, ZP)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_Y, 0x2F);
	mem[0xFFFC] = STY_ZP;
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

TEST(TEST_STY, ZPX)
{
	cpu.Reset();

	cpu.SetRegister(REGISTER_Y, 0x42);
	cpu.X = 0x0F;

	mem[0xFFFC] = STY_ZPX;
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

TEST(TEST_STY, ABS)
{
	cpu.Reset();
	cpu.SetRegister(REGISTER_Y, 0x2F);

	mem[0xFFFC] = STY_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x800F] = 0x00;

	CPU CPUCopy = cpu;

	const int _cycle = 4;
	int cycle = _cycle;
	int CyclesUsed = RunMachine(cycle);

	EXPECT_EQ(mem[0x8000], 0x2F);
	EXPECT_EQ(CyclesUsed, _cycle);

	CheckCPUFlagModified(cpu, CPUCopy);
}

