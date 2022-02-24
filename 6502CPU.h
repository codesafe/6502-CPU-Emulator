#ifndef CPU_H
#define CPU_H

/*
	MOS 6502 CPU Emulator
*/

#include "Predef.h"

class CPU
{

private:
	// Registor
	BYTE	A;		// Accumulator
	BYTE	X;		// Index Registor
	BYTE	Y;
	BYTE	PS;		// Processor Status : Flag
	BYTE	SP;		// Stack Pointer
	WORD	PC;		// program control

public:
	CPU();
	~CPU();

	void Reset();

};

#endif