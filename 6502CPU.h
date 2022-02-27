#ifndef CPU_H
#define CPU_H

/*
	MOS 6502 CPU Emulator
*/

#include "Predef.h"

class Memory;

#define REGISTER_A		0
#define REGISTER_X		1
#define REGISTER_Y		2

/*
BYTE C : 1;	//0: Carry Flag
BYTE Z : 1;	//1: Zero Flag
BYTE I : 1; //2: Interrupt disable
BYTE D : 1; //3: Decimal mode
BYTE B : 1; //4: Break
//BYTE Unused : 1; //5: Unused
BYTE V : 1; //6: Overflow
BYTE N : 1; //7: Negative
*/
// Flag Define : PS
#define CARRY_FLAG				0b00000001
#define ZERO_FLAG				0b00000010
#define INTERRUPT_DISABLE		0b00000100
#define DECIMAL_MODE			0b00001000
#define BREAK					0b00010000
#define OVERFLOW				0b00100000
#define UNUSED					0b01000000
#define NEGATIVE				0b10000000

// Instruction
/*
	구현된것들은 () 처리
	ADC AND ASL BCC BCS BEQ BIT BMI BNE BPL BRK BVC BVS CLC CLD CLI CLV CMP
	CPX CPY DEC DEX DEY EOR INC INX INY JMP JSR (LDA) LDX LDY LSR NOP ORA PHA
	PHP PLA PLP ROL ROR RTI RTS SBC SEC SED SEI STA STX STY TAX TAY TSX TXA
	TXS TYA
*/

// LDA (LoaD Accumulator)
#define LDA_IM		0xA9
#define LDA_ZP		0xA5
#define LDA_ZPX		0xB5
#define LDA_ABS		0xAD
#define LDA_ABSX	0xBD
#define LDA_ABSY	0xB9
#define LDA_INDX	0xA1
#define LDA_INDY	0xB1

// LDX (LoaD X register)
#define LDX_IM		0xA2
#define LDX_ZP		0xA6
#define LDX_ZPY		0xB6
#define LDX_ABS		0xAE
#define LDX_ABSY	0xBE


// JSR (Jump to Subroutine)
#define JSR			0x20


struct StatusFlags
{
	BYTE C : 1;	//0: Carry Flag	
	BYTE Z : 1;	//1: Zero Flag
	BYTE I : 1; //2: Interrupt disable
	BYTE D : 1; //3: Decimal mode
	BYTE B : 1; //4: Break
	BYTE Unused : 1; //5: Unused
	BYTE V : 1; //6: Overflow
	BYTE N : 1; //7: Negative
};

class CPU
{
private:
	// Registor
	BYTE	A;		// Accumulator
	BYTE	X;		// Index Registor
	BYTE	Y;
	//BYTE	PS;		// Processor Status : Flag
	WORD	SP;		// Stack Pointer
	WORD	PC;		// program control

	union
	{
		BYTE PS;
		StatusFlags Flag;
	};

public:
	CPU();
	~CPU();

	void Reset();
	void Run(Memory& mem, int& cycle);

	void SetRegister(BYTE type, BYTE value);
	BYTE GetRegister(BYTE type);

	// Flag
	bool GetFlag(BYTE flag);
	void SetFlag(BYTE flag, bool set);

	void SetZeroNegative(BYTE Register);
	BYTE Fetch(Memory& mem, int& cycle);

	BYTE ReadMem(Memory& mem, WORD add, int& cycle);
	WORD ReadWordMem(Memory& mem, WORD addr, int& cycle);

	WORD FetchWord(Memory& mem, int& cycle);
	void WordWriteMem(Memory& mem, WORD value, int addr, int& cycle);

};

#endif