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



#define AND_ZP		0x25
#define AND_ZPX		0x35
#define AND_ABS		0x2D
#define AND_ABSX	0x3D
#define AND_ABSY	0x39
#define AND_INDX	0x21
#define AND_INDY	0x31
