#include "UnitTestMain.h"

TEST(TEST_INCREASE_DECREASE, INST_INX)
{
	cpu.Reset();
	cpu.X = 0x53;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = INX;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.X, 0x54);

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

#define ADC_IM		0x69
#define ADC_ZP		0x65
#define ADC_ZPX		0x75
#define ADC_ABS		0x6D
#define ADC_ABSX	0x7D
#define ADC_ABSY	0x79
#define ADC_INDX	0x61
#define ADC_INDY	0x71

#define SBC_IM		0xE9
#define SBC_ZP		0xE5
#define SBC_ZPX		0xF5
#define SBC_ABS		0xED
#define SBC_ABSX	0xFD
#define SBC_ABSY	0xF9
#define SBC_INDX	0xE1
#define SBC_INDY	0xF1

#define CMP_IM		0xC9
#define CMP_ZP		0xC5
#define CMP_ZPX		0xD5
#define CMP_ABS		0xCD
#define CMP_ABSX	0xDD
#define CMP_ABSY	0xD9
#define CMP_INDX	0xC1
#define CMP_INDY	0xD1

#define CPX_IM		0xE0
#define CPX_ZP		0xE4
#define CPX_ABS		0xEC

#define CPY_IM		0XC0
#define CPY_ZP		0XC4
#define CPY_ABS		0XCC
