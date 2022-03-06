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
	https://www.cs.jhu.edu/~phi/csf/slides/lecture-6502-stack.pdf
	6502 Stack은 0x01FF, 0x01FE, 0x01FD --> 0x0100로 Push 된다
	Stack은 총 256 Byte
*/
#define STACK_ADDRESS	0x0100
#define STACK_POS		0xFF
#define PC_START		0xFFFC


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
#define FLAG_CARRY					0b00000001
#define FLAG_ZERO					0b00000010
#define FLAG_INTERRUPT_DISABLE		0b00000100
#define FLAG_DECIMAL_MODE			0b00001000
#define FLAG_BREAK					0b00010000
#define FLAG_OVERFLOW				0b00100000
#define FLAG_UNUSED					0b01000000
#define FLAG_NEGATIVE				0b10000000

// Instruction
/*
	구현된것들은 () 처리
	ADC AND ASL BCC BCS BEQ BIT BMI BNE BPL BRK BVC BVS CLC CLD CLI CLV CMP
	CPX CPY DEC DEX DEY EOR INC INX INY (JMP) (JSR) (LDA) (LDX) (LDY) LSR (NOP) ORA (PHA)
	(PHP) (PLA) (PLP) ROL ROR RTI (RTS) SBC SEC SED SEI (STA) (STX) (STY) TAX TAY TSX TXA
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

// LDY (LoaD Y register)
#define LDY_IM		0xA0
#define LDY_ZP		0xA4
#define LDY_ZPX		0xB4
#define LDY_ABS		0xAC
#define LDY_ABSX	0xBC

// No Operation
#define NOP			0xEA

// JSR (Jump to Subroutine)
#define JSR			0x20
#define JMP_ABS		0x4C
#define JMP_IND		0x6C
// Return from Subroutine
#define RTS			0x60


// STA - Store Accumulator
#define STA_ZP		0x85
#define STA_ZPX		0x95
#define STA_ABS		0x8D
#define STA_ABSX	0x9D
#define STA_ABSY	0x99
#define STA_INDX	0x81
#define STA_INDY	0x91

// STX - Store X Register
#define STX_ZP		0x86
#define STX_ZPY		0x96
#define STX_ABS		0x8E

// STY - Store Y Register
#define STY_ZP		0x84
#define STY_ZPX		0x94
#define STY_ABS		0x8C


// Stack Operation
#define TSX			0xBA		// Transfer Stack Pointer to X
#define TXS			0x9A		// Transfer X to Stack Pointer
#define PHA			0X48		// Push Accumulator
#define PLA			0x68		// Pull Accumulator
#define PLP			0x28		// Pull Processor Status
#define PHP			0x08		// Pushes a copy of the status flags on to the stack.

// Logical Operation
#define AND_IM		0x29
#define AND_ZP		0x25
#define AND_ZPX		0x35
#define AND_ABS		0x2D
#define AND_ABSX	0x3D
#define AND_ABSY	0x39
#define AND_INDX	0x21
#define AND_INDY	0x31

#define ORA_IM		0x09
#define ORA_ZP		0x05
#define ORA_ZPX		0x15
#define ORA_ABS		0x0D
#define ORA_ABSX	0x1D
#define ORA_ABSY	0x19
#define ORA_INDX	0x01
#define ORA_INDY	0x11

#define EOR_IM		0x49
#define EOR_ZP		0x45
#define EOR_ZPX		0x55
#define EOR_ABS		0x4D
#define EOR_ABSX	0x5D
#define EOR_ABSY	0x59
#define EOR_INDX	0x41
#define EOR_INDY	0x51


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
public:
	// Registor
	BYTE	A;		// Accumulator
	BYTE	X;		// Index Registor
	BYTE	Y;
	//BYTE	PS;		// Processor Status : Flag
	BYTE	SP;		// Stack Pointer
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
	void SetPCAddress(WORD addr);
	int Run(Memory& mem, int& cycle);

	void SetRegister(BYTE type, BYTE value);
	BYTE GetRegister(BYTE type);

	// Flag
	bool GetFlag(BYTE flag);
	void SetFlag(BYTE flag, bool set);

	void SetZeroNegative(BYTE Register);
	BYTE Fetch(Memory& mem, int& cycle);
	WORD FetchWord(Memory& mem, int& cycle);

	BYTE ReadByte(Memory& mem, WORD add, int& cycle);
	WORD ReadWord(Memory& mem, WORD addr, int& cycle);

	void WriteByte(Memory& mem, BYTE value, int addr, int& cycle);
	void WriteWord(Memory& mem, WORD value, int addr, int& cycle);

	void PushStackByte(Memory& mem, BYTE value, int& cycle);
	void PushStackWord(Memory& mem, WORD value, int& cycle);
	BYTE PopStackByte(Memory& mem, int& cycle);
	WORD PopStackWord(Memory& mem, int& cycle);

	//////////////////////////////////////////////////////////////////////////

	void LoadToRegister(Memory& mem, int& cycle, BYTE& reg);
	void LoadToRegisterFromZP(Memory& mem, int& cycle, BYTE& reg);

	WORD GetStackAddress();
};

#endif