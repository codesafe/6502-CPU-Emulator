/*
	MOS 6502 CPU Emulator
*/
#include <assert.h>
#include "6502CPU.h"
#include "6502Memory.h"

CPU::CPU()
{
	Reset();
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
	PS = 0;			// processor status (flags)
	SP = 0x0100;	// Stack pointer
	PC = 0xFFFC;	// program control
}

void CPU::SetRegister(BYTE type, BYTE value)
{
	switch (type)
	{
		case REGISTER_A :
			A = value;
			break;
		case REGISTER_X:
			X = value;
			break;
		case REGISTER_Y:
			Y = value;
			break;
	}
}

BYTE CPU::GetRegister(BYTE type)
{
	switch (type)
	{
	case REGISTER_A:
		return A;
	case REGISTER_X:
		return X;
	case REGISTER_Y:
		return Y;
	}

	assert(-1);
	return 0;
}

void CPU::SetFlag(BYTE flag, bool set)
{
	if (set)
		PS |= flag;
	else
		PS &= ~flag;
}

bool CPU::GetFlag(BYTE flag)
{
	return PS & flag;
}


void CPU::SetZeroNegative(BYTE Register)
{
	// A가 0 이면 Zero flag
	//SetFlag(ZERO_FLAG, A == 0);
	Flag.Z = (Register == 0);

	// A가 Negative Flag 면 Negative flag Set
	//SetFlag(NEGATIVE, A & NEGATIVE);
	Flag.N = (Register & NEGATIVE) > 0;
}

BYTE CPU::Fetch(Memory& mem, int &cycle)
{
	BYTE c = mem.GetByte(PC++);
	cycle--;
	return c;
}

WORD CPU::FetchWord(Memory& mem, int& cycle)
{
	BYTE c0 = mem.GetByte(PC++);
	BYTE c1 = mem.GetByte(PC++);

	// 엔디안에 따라 c0 <--> c1해야 할수도 있다
	WORD w = c1 << 8 | c0;
	cycle-=2;
	return w;
}

// 메모리에서 읽는데 cycle소모 x / PC무관 할때 (Zero page같은것)
BYTE CPU::ReadMem(Memory& mem, WORD addr, int& cycle)
{
	BYTE c = mem.GetByte(addr);
	cycle--;
	return c;
}

WORD CPU::ReadWordMem(Memory& mem, WORD addr, int& cycle)
{
	WORD c = mem.ReadWord(addr);
	cycle -= 2;
	return c;
}

void CPU::WordWriteMem(Memory& mem, WORD value, int addr, int& cycle)
{
	mem.WriteWord(value, addr);
	cycle-=2;
}

void CPU::Run(Memory &mem, int &cycle)
{
	while (cycle > 0)
	{
		BYTE inst = Fetch(mem, cycle);

		switch (inst)
		{
			case LDA_IM: // 2 cycle
			{
				A = Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ZP: // 3 cycle
			{
				// $0000 to $00FF
				// Zero page에서 읽어서 A로
				BYTE zpa = Fetch(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadMem(mem, zpa, cycle);

				SetZeroNegative(A);
			}
			break;

			case LDA_ZPX : // 4 cycle
			{
				// Zero page의 주소와 X 레지스터를 더한 주소에서 읽어 A로..
				// X 레지스터에 $0F이고 LDA $80, X 이면 $80+$0F = $8F에서 A로 읽게됨
				BYTE zpa = Fetch(mem, cycle);
				zpa += X;
				cycle--;
				// Zero page읽으면서 cycle 소모
				A = ReadMem(mem, zpa, cycle);

				SetZeroNegative(A);
			}
			break;

			// 절대 주소 지정을 사용하는 명령어는 대상 위치를 식별하기 위해 전체 16 비트 주소를 포함합니다.
			case LDA_ABS: // 4 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				A = ReadMem(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSX:// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				// 메모리엑세스 페이지를 넘어가면 추가 사이클이 소요됨 (하드웨어가 그렇게 만들어짐?)
				WORD addr = FetchWord(mem, cycle);
				if ( (addr + X) - addr >= 0xFF )
					cycle--;	// page 넘어감

				A = ReadMem(mem, addr + X, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSY:
			{
				WORD addr = FetchWord(mem, cycle);
				if ((addr + Y) - addr >= 0xFF)
					cycle--;	// page 넘어감

				A = ReadMem(mem, addr, cycle);
				SetZeroNegative(A);

			}
			break;

			case LDA_INDX:
			{
				BYTE t = Fetch(mem, cycle);
				WORD inx = t + X;

				WORD addr = ReadWordMem(mem, inx, cycle);
				A = ReadMem(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case LDA_INDY:
				break;

			//////////////////////////////////////////////////////////////////////////////

			// LDX
			case LDX_IM:
			{
				X = Fetch(mem, cycle);
				// X가 0 이면 Zero flag
				SetFlag(ZERO_FLAG, A == 0);
				// X가 Negative Flag 면 Negative flag Set
				SetFlag(NEGATIVE, A & NEGATIVE);

			}
			break;

			case LDX_ZP:
				break;

			case LDX_ZPY:
				break;

			case LDX_ABS:
				break;

			case LDX_ABSY:
				break;


			case JSR : // 6 cycle
			{
				// The JSR instruction pushes the address (minus one) of the return 
				// point on to the stack and then sets the program counter to the target memory address.
  				WORD sr_addr = FetchWord(mem, cycle);
				// PC - 1의 값을 SP가 가르키는 메모리 위치에 쓰기 ( 2cycle소모)
				WordWriteMem(mem, PC - 1, SP, cycle);

				PC = sr_addr;
				cycle--;
			}
			break;


			default:
				printf("Unknown instruction : %x", inst);
				break;
		}
	}
}