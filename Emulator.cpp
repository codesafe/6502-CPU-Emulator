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

// LDA
#define LDA_IM		0xA9
#define LDA_ZP		0xA5
#define LDA_ZPX		0xB5
#define LDA_ABS		0xAD
#define LDA_ABSX	0xBD
#define LDA_ABSY	0xB9
#define LDA_INDX	0xA1
#define LDA_INDY	0xB1


//////////////////////////////////////	Memory

// Memory
BYTE memory[MEMORY_SIZE];

void InitMemory()
{
	memset(memory, 0, MEMORY_SIZE);
}

//////////////////////////////////////	CPU

// Registor
BYTE	A;		// Accumulator
BYTE	X;		// Index Registor
BYTE	Y;
BYTE	PS;		// Processor Status : Flag
BYTE	SP;		// Stack Pointer
WORD	PC;		// program control


void InitCPU()
{
	A = 0;
	X = 0;
	Y = 0;
	SP = 0;
	PC = 0;
}

// 명령어를 Memory --> Cpu로 가져온다. PC증가 , Cycle 감소
BYTE FetchByte(int &cycle)
{
	BYTE dat = memory[PC];
	PC++;
	cycle--;
	return dat;
}

void SetFlag(BYTE flag, bool set)
{
	if( set )
		SP |= (0x01 << flag);
	else
		SP &= ~(0x01 << flag);
}

bool GetFlag(BYTE flag)
{
	return SP & (0x01 << flag);
}

//////////////////////////////////////	





void Run(int cycle)
{
	while (cycle > 0)
	{
		// Get instruction from memory
		BYTE inst = FetchByte(cycle);

		if (inst == LDA_IM)
		{
			// Loads a byte of memory into the accumulator setting the zeroand negative flags as appropriate.
			// LDA #10 : Load 10 ($0A) into the accumulator
			A = FetchByte(cycle);
			// A가 0 이면 Zero flag
			SetFlag(ZERO_FLAG, A == 0);

			// A가 Negative Flag 면 Negative flag Set
			SetFlag(NEGATIVE, A & NEGATIVE);
		}

	}
}


int main()
{
	InitMemory();
	InitCPU();

	// TEST : 명령어 데이터 셋


	Run(2);

	return 0;
}
