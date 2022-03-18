#include "UnitTestMain.h"

TEST(TEST_ARITHMETIC, INST_ADC_IM)
{
	cpu.Reset();

	cpu.A = 0b10110001;		// B1
	cpu.Flag.C = false;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = ADC_IM;
	mem[0xFFFD] = 0b01111001;	// 79

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0x2A);	// 12A

	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.V);
}


TEST(TEST_ARITHMETIC, INST_ADC_ZP)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = -1;


	CPU CPUCopy = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}

TEST(TEST_ARITHMETIC, INST_ADC_ZPX)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.X = 0x04;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_ZPX;
	mem[0xFFFD] = 0x10;
	mem[0x0014] = -1;


	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}


TEST(TEST_ARITHMETIC, INST_ADC_ABS)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = -1;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}


TEST(TEST_ARITHMETIC, INST_ADC_ABSX)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.X = 0x04;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = -1;


	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}

TEST(TEST_ARITHMETIC, INST_ADC_ABSY)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.Y = 0x04;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = -1;


	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}


TEST(TEST_ARITHMETIC, INST_ADC_INDX)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.X = 4;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_INDX;
	mem[0xFFFD] = 0x02;

	mem[0x0006] = 0x00;	// 0x02 + 0x04
	mem[0x0007] = 0x80;

	mem[0x8000] = -1;


	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}

TEST(TEST_ARITHMETIC, INST_ADC_INDY)
{
	cpu.Reset();

	cpu.A = -128;
	cpu.Y = 0xF4;
	cpu.Flag.C = false;

	mem[0xFFFC] = ADC_INDY;
	mem[0xFFFD] = 0x02;

	mem[0x0002] = 0xF0;	
	mem[0x0003] = 0x80;

	mem[0x80F0 + cpu.Y] = -1;	// 0x8000 + Y reg

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 127);

	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
}

//////////////////////////////////////////////////////////////////////////

TEST(TEST_ARITHMETIC, INST_SBC_IM)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_IM;
	mem[0xFFFD] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}




TEST(TEST_ARITHMETIC, INST_SBC_ZP)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_SBC_ZPX)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.X = 0x04;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_ZPX;
	mem[0xFFFD] = 0x10;
	mem[0x0014] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}


TEST(TEST_ARITHMETIC, INST_SBC_ABS)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}


TEST(TEST_ARITHMETIC, INST_SBC_ABSX)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.X = 0x04;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = 0;


	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_SBC_ABSY)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Y = 0x04;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = 0;

	CPU CPUCopy = cpu;
	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}


TEST(TEST_ARITHMETIC, INST_SBC_INDX)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.X = 4;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_INDX;
	mem[0xFFFD] = 0x02;

	mem[0x0006] = 0x00;	// 0x02 + 0x04
	mem[0x0007] = 0x80;

	mem[0x8000] = 0;


	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_SBC_INDY)
{
	cpu.Reset();

	cpu.A = 0;
	cpu.Y = 0xF4;
	cpu.Flag.C = true;
	cpu.Flag.Z = cpu.Flag.N = true;

	mem[0xFFFC] = SBC_INDY;
	mem[0xFFFD] = 0x02;

	mem[0x0002] = 0xF0;
	mem[0x0003] = 0x80;

	mem[0x80F0 + cpu.Y] = 0;	// 0x8000 + Y reg

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_EQ(cpu.A, 0);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_TRUE(cpu.Flag.C);
}


//////////////////////////////////////////////////////////////////////////

TEST(TEST_ARITHMETIC, INST_CMP_IM)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CMP_IM;
	mem[0xFFFD] = 26;

	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
	//EXPECT_TRUE(cpu.Flag.V);
}


TEST(TEST_ARITHMETIC, INST_CMP_ZP)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CMP_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 26;


	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CMP_ZPX)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.X = 0x04;

	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CMP_ZPX;
	mem[0xFFFD] = 0x10;
	mem[0x0014] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CMP_ABS)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;


	mem[0xFFFC] = CMP_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CMP_ABSX)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.X = 0x04;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;


	mem[0xFFFC] = CMP_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CMP_ABSY)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.Y = 0x04;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;


	mem[0xFFFC] = CMP_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;

	mem[0x4484] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CMP_INDX)
{
	cpu.Reset();

	cpu.A = 26;
	cpu.X = 4;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CMP_INDX;
	mem[0xFFFD] = 0x02;

	mem[0x0006] = 0x00;	// 0x02 + 0x04
	mem[0x0007] = 0x80;

	mem[0x8000] = 26;

	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}


TEST(TEST_ARITHMETIC, INST_CMP_INDY)
{
	cpu.Reset();

	cpu.A = 28;
	cpu.Y = 0xF4;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CMP_INDY;
	mem[0xFFFD] = 0x02;

	mem[0x0002] = 0xF0;
	mem[0x0003] = 0x80;

	mem[0x80F0 + cpu.Y] = 28;

	CPU CPUCopy = cpu;
	const int _cycle = 6;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}


//////////////////////////////////////////////////////////////////////////

TEST(TEST_ARITHMETIC, INST_CPX_IM)
{
	cpu.Reset();

	cpu.X = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CPX_IM;
	mem[0xFFFD] = 26;

	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CPX_ZP)
{
	cpu.Reset();

	cpu.X = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CPX_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 26;


	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CPX_ABS)
{
	cpu.Reset();

	cpu.X = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;


	mem[0xFFFC] = CPX_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}



//////////////////////////////////////////////////////////////////////////

TEST(TEST_ARITHMETIC, INST_CPY_IM)
{
	cpu.Reset();

	cpu.Y = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CPY_IM;
	mem[0xFFFD] = 26;

	const int _cycle = 2;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CPY_ZP)
{
	cpu.Reset();

	cpu.Y = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;

	mem[0xFFFC] = CPY_ZP;
	mem[0xFFFD] = 0x10;
	mem[0x0010] = 26;


	const int _cycle = 3;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);

	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

TEST(TEST_ARITHMETIC, INST_CPY_ABS)
{
	cpu.Reset();

	cpu.Y = 26;
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;


	mem[0xFFFC] = CPY_ABS;
	mem[0xFFFD] = 0x00;
	mem[0xFFFE] = 0x80;

	mem[0x8000] = 26;

	const int _cycle = 4;
	int usedcycle = RunMachine(_cycle);

	EXPECT_EQ(usedcycle, _cycle);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.C);
}

