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
	PS = 0;				// processor status (flags)
	SP = STACK_POS;	// Stack pointer
	PC = PC_START;		// program control
}

void CPU::SetPCAddress(WORD addr)
{
	PC = addr;
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
	Flag.N = (Register & FLAG_NEGATIVE) > 0;
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
	WORD w = (c1 << 8) | c0;
	cycle-=2;
	return w;
}

// 메모리에서 읽는데 cycle소모 x / PC무관 할때 (Zero page같은것)
BYTE CPU::ReadByte(Memory& mem, WORD addr, int& cycle)
{
	BYTE c = mem.GetByte(addr);
	cycle--;
	return c;
}

WORD CPU::ReadWord(Memory& mem, WORD addr, int& cycle)
{
	WORD c = mem.ReadWord(addr);
	cycle -= 2;
	return c;
}

void CPU::WriteByte(Memory& mem, BYTE value, int addr, int& cycle)
{
	mem.SetByte(addr, value);
	cycle --;
}

void CPU::WriteWord(Memory& mem, WORD value, int addr, int& cycle)
{
	mem.WriteWord(value, addr);
	cycle-=2;
}

// 이거를 써야하는 이유는 
/*
	SP는 1byte이고 Stack 메모리는 0x01FF -> 0x0100까지 256 Byte이므로
	Address는 WORD이고 스택의 메모리 위치는 감소하기 때문에 이렇게 계산해햐함
*/
WORD CPU::GetStackAddress()
{
	WORD sp = STACK_ADDRESS | SP;
	return sp;
}

// Byte를 Stack에 Push
void CPU::PushStackByte(Memory& mem, BYTE value, int& cycle)
{
	WriteByte(mem, value, GetStackAddress(), cycle);
	SP--;
}

// Word를 Stack에 Push
void CPU::PushStackWord(Memory& mem, WORD value, int& cycle)
{
	WriteByte(mem, value >> 8, GetStackAddress(), cycle);
	SP--;
	WriteByte(mem, value & 0xFF, GetStackAddress(), cycle);
	SP--;
}

// 스택에서 1 byte POP
BYTE CPU::PopStackByte(Memory& mem, int& cycle)
{
	SP++;
	BYTE popbyte = ReadByte(mem, GetStackAddress(), cycle);
	cycle--;
	return popbyte;
}

// Stack에서 Word pop
WORD CPU::PopStackWord(Memory& mem, int& cycle)
{
	WORD popWord = ReadWord(mem, GetStackAddress()+1, cycle);
	SP += 2;
	cycle--;
	return popWord;
}


int CPU::Run(Memory &mem, int &cycle)
{
	const int CyclesRequested = cycle;

	while (cycle > 0)
	{
		// 여기에서 cycle 하나 소모
		BYTE inst = Fetch(mem, cycle);

		switch (inst)
		{
			case LDA_IM: // 2 cycle
			{
				//LoadToRegister(mem, cycle, A);
				A = Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ZP: // 3 cycle
			{
				//LoadToRegisterFromZP(mem, cycle, A);

				// $0000 to $00FF
				// Zero page에서 읽어서 A로
				BYTE zpa = Fetch(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadByte(mem, zpa, cycle);
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
				A = ReadByte(mem, zpa, cycle);

				SetZeroNegative(A);
			}
			break;

			// 절대 주소 지정을 사용하는 명령어는 대상 위치를 식별하기 위해 전체 16 비트 주소를 포함합니다.
			case LDA_ABS: // 4 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSX:// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				// 메모리엑세스 페이지를 넘어가면 추가 사이클이 소요됨 (하드웨어가 그렇게 만들어짐?)
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);
				
				WORD t = lo + X;
				if( t > 0xFF ) cycle--;
				WORD addr = (lo | (hi << 8)) + X;

// 				WORD addr = FetchWord(mem, cycle);
// 				if ( (addr + X) - addr >= 0xFF )
// 					cycle--;	// page 넘어감

				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSY:	// 4 cycle
			{
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);

				WORD t = lo + Y;
				if (t > 0xFF) cycle--;
				WORD addr = (lo | (hi << 8)) + Y;

				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);

			}
			break;

			case LDA_INDX:	// 6 cycle
			{
				BYTE t = Fetch(mem, cycle);
				WORD inx = t + X;
				cycle--;
				WORD addr = ReadWord(mem, inx, cycle);
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case LDA_INDY: // 5 ~ 6 cycle
			{
				// zero page에서 word 읽고 Y레지스터와 더한 주소의 1바이트를 A에 로드
				// 읽을 주소가 page를 넘으면 1사이클 감소
				BYTE addr = Fetch(mem, cycle);
				BYTE lo = ReadByte(mem, addr, cycle);
				BYTE hi = ReadByte(mem, addr+1, cycle);

				WORD t = lo + Y;
				if (t > 0xFF) cycle--;	// page 넘어감

				WORD index_addr = (lo | (hi << 8)) + Y;
				A = ReadByte(mem, index_addr, cycle);

				SetZeroNegative(A);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// LDX
			case LDX_IM:	// 2cycle
			{
				//LoadToRegister(mem, cycle, X);
				X = Fetch(mem, cycle);
				SetZeroNegative(X);
			}
			break;

			case LDX_ZP:	// 3cycle
			{
				//LoadToRegisterFromZP(mem, cycle, X);
 				BYTE zpa = Fetch(mem, cycle);
 				X = ReadByte(mem, zpa, cycle);
 				SetZeroNegative(X);
			}
			break;

			case LDX_ZPY:	// 4cycle
			{
				BYTE zpage = Fetch(mem, cycle);
				zpage += Y;
				cycle--;
				X = ReadByte(mem, zpage, cycle);
				SetZeroNegative(X);
			}
			break;

			case LDX_ABS:	// 4cycle
			{
				WORD addr = FetchWord(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			case LDX_ABSY:	// 4 ~ 5 cycle
			{
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);

				WORD t = lo + Y;
				if (t > 0xFF) cycle--;
				WORD addr = (lo | (hi << 8)) + Y;

				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case LDY_IM : // 2 cycle
			{
				//LoadToRegister(mem, cycle, Y);
				Y = Fetch(mem, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZP: // 3 cycle
			{
				//LoadToRegisterFromZP(mem, cycle, Y);
				BYTE zpa = Fetch(mem, cycle);
				Y = ReadByte(mem, zpa, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZPX : // 4 cycle
			{
				BYTE zpage = Fetch(mem, cycle);
				zpage += X;
				cycle--;
				Y = ReadByte(mem, zpage, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABS : // 4 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABSX : // 4~5 cycle
			{
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);

				WORD t = lo + X;
				if (t > 0xFF) cycle--;
				WORD addr = (lo | (hi << 8)) + X;

				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STA_ZP	:	// 3 cycle
			{
				// ZeroPage에 A레지스터 내용 쓰기
				BYTE zpage = Fetch(mem, cycle);
				WriteByte(mem, A, zpage, cycle);
			}
			break;

			case STA_ZPX :	// 4 cycle
			{
				// ZP + X에 A레지스터 내용쓰기
				BYTE zpage = Fetch(mem, cycle);
				zpage += X;
				cycle--;
				WriteByte(mem, A, zpage, cycle);
			}
			break;

			case STA_ABS:	// 4 cycle
			{
				// WORD address에 A레지스터 내용 쓰기
				WORD addr = FetchWord(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSX:	// 5 cycle
			{
				// WORD address + X에 A레지스터 내용 쓰기
				WORD addr = FetchWord(mem, cycle);
				addr += X;
				cycle--;
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSY:	// 5 cycle
			{
				// WORD address + Y에 A레지스터 내용 쓰기
				WORD addr = FetchWord(mem, cycle);
				addr += Y;
				cycle--;
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_INDX:	// 6 cycle
			{
				// ZeroPage + X 가 가르키는곳에 A레지스터 내용쓰기 
				BYTE zp = Fetch(mem, cycle);
				zp += X;
				cycle--;
				WORD addr = ReadWord(mem, zp, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_INDY:	// 6 cycle
			{
				// ZeroPage에서 WORD address얻고 address + Y에 가르키는곳에 A레지스터 내용쓰기 
				BYTE zp = Fetch(mem, cycle);
				WORD addr = ReadWord(mem, zp, cycle);
				addr += Y;
				cycle--;
				WriteByte(mem, A, addr, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STX_ZP	:	// 3 cycle
			{
				// ZeroPage에 X레지스터 내용 쓰기
				BYTE zpage = Fetch(mem, cycle);
				WriteByte(mem, X, zpage, cycle);
			}
			break;

			case STX_ZPY:	// 4 cycle
			{
				// ZP + X에 X레지스터 내용쓰기
				BYTE zpage = Fetch(mem, cycle);
				zpage += Y;
				cycle--;
				WriteByte(mem, X, zpage, cycle);
			}
			break;

			case STX_ABS:	// 4 cycle
			{
				// WORD address에 X레지스터 내용 쓰기
				WORD addr = FetchWord(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STY_ZP:	// 3 cycle
			{
				// ZeroPage에 X레지스터 내용 쓰기
				BYTE zpage = Fetch(mem, cycle);
				WriteByte(mem, Y, zpage, cycle);

			}
			break;

			case STY_ZPX:	// 4 cycle
			{
				// ZP + X에 Y레지스터 내용쓰기
				BYTE zpage = Fetch(mem, cycle);
				zpage += X;
				cycle--;
				WriteByte(mem, Y, zpage, cycle);
			}
			break;

			case STY_ABS:	// 4 cycle
			{
				// WORD address에 Y레지스터 내용 쓰기
				WORD addr = FetchWord(mem, cycle);
				WriteByte(mem, Y, addr, cycle);
			}
			break;



			////////////////////////////////////////////////////////////////////////////// JUMP

			case JSR : // 6 cycle
			{
				// The JSR instruction pushes the address (minus one) of the return 
				// point on to the stack and then sets the program counter to the target memory address.
  				WORD sr_addr = FetchWord(mem, cycle);

				// 스택에 PC-1을 Push
				PushStackWord(mem, PC - 1, cycle);

				PC = sr_addr;
				cycle--;
			}
			break;

			case JMP_ABS :	// 3 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				PC = addr;
			}
			break;

			case JMP_IND :	// 5 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				addr = ReadWord(mem, addr, cycle);
				PC = addr;
			}
			break;

			case RTS :	// 6 cycle
			{
				WORD addr = PopStackWord(mem, cycle);
				PC = addr + 1;
				cycle -= 2;
			}
			break;

			//////////////////////////////////////////////////////////////////////////////	STACK

			// Transfer (Stack Pointer) to X
			case TSX :	// 2 cycle
			{
				// 스택포인터를 X 레지스터로
				X = SP;
				cycle--;
				// Z / N flag
				SetZeroNegative(X);
			}
			break;

			// Transfer X to (Stack Pointer)
			case TXS :	// 2 cycle
			{
				// X레지스터를 SP로
				SP = X;
				cycle--;
			}
			break;

			// Pushes a copy of the accumulator on to the stack.
			case PHA :	// 3 cycle
			{
				// A 레지스터를 스택에 Push
				PushStackByte(mem, A, cycle);
				cycle--;
			}
			break;

			// Pulls an 8 bit value from the stack and into the accumulator. 
			// The zero and negative flags are set as appropriate.
			case PLA :	// 4 cycle
			{
				// 스택에서 8비트를 pull --> A로
				A = PopStackByte(mem, cycle);
				cycle--;
				// Z / N flag
				SetZeroNegative(A);
			}
			break;

			// Pulls an 8 bit value from the stack and into the processor flags. 
			// The flags will take on new states as determined by the value pulled.
			case PLP :	// 4 cycle
			{
				// pop 8 bit를 --> PS (Flag) : 플레그들은 Pop된 값에의하여 새로운 플레그 상태를 갖음
				PS = PopStackByte(mem, cycle);
				cycle--;
				// B , Unused는 사용하지 않음
				Flag.B = false;
				Flag.Unused = false;
			}
			break;


			//////////////////////////////////////////////////////////////////////////////

			case NOP :
				cycle--;
			break;

			//////////////////////////////////////////////////////////////////////////////

			default:
				printf("Unknown instruction : %x", inst);
				break;
		}
	}

	return CyclesRequested - cycle;
}

void CPU::LoadToRegister(Memory& mem, int& cycle, BYTE &reg)
{
	reg = Fetch(mem, cycle);
	SetZeroNegative(reg);
}

// ZP에 있는 값을 레지스터에 로드
void CPU::LoadToRegisterFromZP(Memory& mem, int& cycle, BYTE& reg)
{
	BYTE zpa = Fetch(mem, cycle);
	reg = ReadByte(mem, zpa, cycle);
	SetZeroNegative(reg);
}
