/*
	MOS 6502 CPU Emulator
*/

#include "6502CPU.h"

CPU::CPU()
{

}

CPU::~CPU()
{

}

// reset all register
void CPU::Reset()
{
	A = 0;
	X = 0;
	Y = 0;
	SP = 0;
	PC = 0;
}

