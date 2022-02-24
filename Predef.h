#ifndef PREDEF_H
#define PREDEF_H


/*
	MOS 6502 CPU Emulator
*/

#include <stdio.h>
#include <memory.h>

#define BYTE		unsigned char
#define WORD		unsigned short
#define MEMORY_SIZE	1024 * 64	// 64k

// Flag Define : PS
#define CARRY_FLAG				0
#define ZERO_FLAG				1
#define INTERRUPT_DISABLE		2
#define DECIMAL_MODE			3
#define BREAK					4
#define OVERFLOW				5
#define UNUSED					6
#define NEGATIVE				7

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

#endif