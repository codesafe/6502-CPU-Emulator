/*
	MOS 6502 CPU Emulator
*/

#pragma once

#include "6502CPU.h"
#include "6502Memory.h"

#define TEST(X)	int TEST_##X(Memory& mem, CPU& cpu)
#define DOTEST(X)	TEST_##X(mem, cpu)

class TEST_LOAD
{
public :
	TEST_LOAD() {};
	~TEST_LOAD() {}

	void TEST_LDA_ZP(Memory& mem, CPU& cpu)
	{
		mem.SetByte(0xFFFC, LDA_ZP);
		mem.SetByte(0xFFFD, 0x10);
		mem.SetByte(0xFFFE, 0xAD);
	}

	void TEST_LDA_ZPX(Memory& mem, CPU& cpu)
	{
		mem.SetByte(0xFFFC, LDA_ZPX);
		mem.SetByte(0xFFFD, 0x10);
	}

	void TEST_JSR(Memory& mem, CPU& cpu)
	{
		mem.SetByte(0xFFFC, JSR);
		mem.SetByte(0xFFFD, 0x40);
		mem.SetByte(0xFFFE, 0x40);

		mem.SetByte(0x4040, LDA_IM);
		mem.SetByte(0x4041, 0x84);

	}

	void TEST_LDA_ABS(Memory& mem, CPU& cpu)
	{
		mem.SetByte(0xFFFC, LDA_ABS);
		mem.SetByte(0xFFFD, 0x80);
		mem.SetByte(0xFFFE, 0x44);

		mem.SetByte(0x4480, 0x88);

	}

	void TEST_LDA_ABSX(Memory& mem, CPU& cpu)
	{
		cpu.SetRegister(REGISTER_X, 1);

		mem.SetByte(0xFFFC, LDA_ABSX);
		mem.SetByte(0xFFFD, 0x80);
		mem.SetByte(0xFFFE, 0x44);

		mem.SetByte(0x4481, 0x88);

	}

	int TEST_LDA_INDX(Memory& mem, CPU& cpu)
	{
		cpu.SetRegister(REGISTER_X, 0x04);

		mem.SetByte(0xFFFC, LDA_INDX);
		mem.SetByte(0xFFFD, 0x02);

		mem.SetByte(0x0006, 0x00);	// 0x02 + 0x04
		mem.SetByte(0x0007, 0x80);

		mem.SetByte(0x8000, 0x77);

		return 6;
	}

	TEST(LDA_INDY)
	{
		cpu.SetRegister(REGISTER_Y, 0xFF);

		mem.SetByte(0xFFFC, LDA_INDY);
		mem.SetByte(0xFFFD, 0x02);

		mem.SetByte(0x0002, 0x02);
		mem.SetByte(0x0003, 0x80);

		mem.SetByte(0x8101, 0x37);

		return 6;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	TEST(LDX_IM)
	{
		return 2;
	}

	TEST(LDX_ZP)
	{
		mem.SetByte(0xFFFC, LDX_ZP);
		mem.SetByte(0xFFFD, 0x10);
		mem.SetByte(0xFFFE, 0xAD);
		return  3;
	}

	TEST(LDX_ZPY)
	{
		mem.SetByte(0xFFFC, LDX_ZPY);
		mem.SetByte(0xFFFD, 0x10);
		return  4;
	}

	TEST(LDX_ABS)
	{
		mem.SetByte(0xFFFC, LDX_ABS);
		mem.SetByte(0xFFFD, 0x80);
		mem.SetByte(0xFFFE, 0x44);

		mem.SetByte(0x4480, 0x88);
		return  4;
	}

	TEST(LDX_ABSY)
	{
		cpu.SetRegister(REGISTER_Y, 1);

		mem.SetByte(0xFFFC, LDX_ABSY);
		mem.SetByte(0xFFFD, 0x80);
		mem.SetByte(0xFFFE, 0x44);

		mem.SetByte(0x4481, 0x88);
		return  5;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	TEST(LDY_IM)
	{
		return  5;
	}

	TEST(LDY_ZP)
	{
		return  5;
	}

	TEST(LDY_ZPX)
	{
		return  5;
	}

	TEST(LDY_ABS)
	{
		return  5;
	}

	TEST(LDY_ABSX)
	{
		return  5;
	}

	///////////////////////////////////////////////////////////////////////////////////////


};

