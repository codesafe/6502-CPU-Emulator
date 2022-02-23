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

////////////////////////////////////////////////////////////////////////////	

// Memory
BYTE* memory = NULL;

// Registor
BYTE	A;		// Accumulator
BYTE	X;		// Index Registor
BYTE	Y;
BYTE	PS;		// Processor Status : Flag
BYTE	SP;		// Stack Pointer
WORD	PC;		// program control

////////////////////////////////////////////////////////////////////////////	Memory

void InitMemory()
{
	memory = new BYTE[MEMORY_SIZE];
	memset(memory, 0, MEMORY_SIZE);
}

void ReleaseMemory()
{
	delete[] memory;
	memory = NULL;
}


////////////////////////////////////////////////////////////////////////////	CPU




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

void SetZeroNegative()
{
	// A가 0 이면 Zero flag
	SetFlag(ZERO_FLAG, A == 0);
	// A가 Negative Flag 면 Negative flag Set
	SetFlag(NEGATIVE, A & NEGATIVE);
}

////////////////////////////////////////////////////////////////////////////	


void Run(int cycle)
{
	while (cycle > 0)
	{
		// CPU 명령어 처리 순서
		// FETCH and increment the Program Counter
		// DECODE against the addressing mode
		// EXECUTE the instruction
		// update ticks count
		 

		// Get instruction from memory
		BYTE inst = FetchByte(cycle);
		switch (inst)
		{
			// LDA
			case LDA_IM :
			{
				// Loads a byte of memory into the accumulator setting the zeroand negative flags as appropriate.
				// LDA #10 : Load 10 ($0A) into the accumulator

				// address node (immediate) : 상수값을 A로 ..
				A = FetchByte(cycle);
				SetZeroNegative();
			}
			break;

			case LDA_ZP :
			{
				// $0000 to $00FF
				// Zero page에서 읽어서 A로
				A = memory[FetchByte(cycle)];
				SetZeroNegative(); // ?? 필요?
			}
			break;

			/*
				인덱스 제로 페이지 어드레싱을 사용하는 명령어에 의해 접근될 주소는 명령어로부터 8비트 제로 
				페이지 어드레스를 가져와서 X 레지스터의 현재 값을 더함으로써 계산된다. 예를 들어, X 레지스터에 
				$0F가 포함되어 있고 LDA $80,X 명령이 실행되면 누적기는 $008F(예: $80 + $0F => $8F)에서 
				로드됩니다.
				NB:주소 계산은 기본 주소와 레지스터의 합계가 $FF를 초과할 경우 래핑됩니다. 
				마지막 예를 반복하지만 X 레지스터에 $FF가 있는 경우 누적기는 $017F가 아니라 
				$007F(예: $80 + $FF => $7F)에서 로드됩니다.
			*/
			case LDA_ZPX :
			{
				BYTE zp = FetchByte(cycle);
				zp += X;	// zp + X 레지스터
				A = memory[zp];
				SetZeroNegative(); // ?? 필요?
			}
			break;

			// 절대 주소 지정을 사용하는 명령어는 대상 위치를 식별하기 위해 전체 16 비트 주소를 포함합니다.
			case LDA_ABS :
			{
				BYTE lo = FetchByte(cycle);
				BYTE hi = FetchByte(cycle);
				WORD addr = (hi << 8) | lo;
				A = memory[addr];
			}
			break;

			/*
				X 레지스터 인덱스 절대 주소 지정을 사용하는 명령어에 의해 접근되는 주소는 명령어로부터 16비트 주소를 
				가져와서 X 레지스터의 내용을 추가함으로써 계산된다. 예를 들어, X에 $92가 포함되어 있으면
				STA $2000,X 명령은 축열조를 $2092(예: $2000 + $92)로 저장합니다.
			*/
			case LDA_ABSX :
			{
				BYTE lo = FetchByte(cycle);
				BYTE hi = FetchByte(cycle);
				WORD addr = ((hi << 8) | lo) + X;
				if ((addr & 0xFF00) != (hi << 8))
				{
					// page 넘어감 / carry bit set ?
				}
				else
				{
					// page 안 넘어감
				}
			}
			break;

			case LDA_ABSY :
			{
				BYTE lo = FetchByte(cycle);
				BYTE hi = FetchByte(cycle);
				WORD addr = ((hi << 8) | lo) + Y;
				if ((addr & 0xFF00) != (hi << 8))
				{
					// page 넘어감 / carry bit set ?
				}
				else
				{
					// page 안 넘어감
				}
			}
			break;

			case LDA_INDX :
			{
				BYTE lo = FetchByte(cycle);
				BYTE hi = FetchByte(cycle);
			}
			break;

			case LDA_INDY :
				break;

			// LDX
			case LDX_IM :
			{
				X = FetchByte(cycle);
				// X가 0 이면 Zero flag
				SetFlag(ZERO_FLAG, A == 0);
				// X가 Negative Flag 면 Negative flag Set
				SetFlag(NEGATIVE, A & NEGATIVE);

			}
			break;

			case LDX_ZP :
				break;

			case LDX_ZPY:
				break;

			case LDX_ABS:
				break;

			case LDX_ABSY:
				break;



			default:
				break;
		}
	}
}

int main()
{
	InitMemory();
	InitCPU();

	// TEST : 명령어 데이터 셋
	Run(2);

	ReleaseMemory();
	return 0;
}
