#include "UnitTestMain.h"

TEST(TEST_TRANSFER_REGISTER, INST_TAX)
{
	cpu.Reset();
	cpu.A = 0x77;
	cpu.X = 0x88;
	cpu.Flag.Z = cpu.Flag.N = true;

	// Transfer Accumulator to X
	mem[0xFFFC] = TAX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, cpu.X);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}


#define INX			0xE8
#define INY			0xC8
#define DEX			0xCA
#define DEY			0x88
#define INC_ZP		0xE6
#define INC_ZPX		0xF6
#define INC_ABS		0xEE
#define INC_ABSX	0xFE
#define DEC_ZP		0xC6
#define DEC_ZPX		0xD6
#define DEC_ABS		0xCE
#define DEC_ABSX	0xDE
