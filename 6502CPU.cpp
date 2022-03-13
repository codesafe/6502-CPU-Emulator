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
	//SetFlag(FLAG_ZERO, Register == 0);
	Flag.Z = (Register == 0);

	//SetFlag(FLAG_NEGATIVE, Register & FLAG_NEGATIVE);
	Flag.N = (Register & FLAG_NEGATIVE) > 0;
}

void CPU::SetCarryFlag(WORD value)
{
	Flag.C = (value & 0xFF00) > 0;
	//SetFlag(FLAG_CARRY, (value & 0xFF00) > 0);
}

void CPU::SetOverflow(BYTE oldv0, BYTE v0, BYTE v1)
{
	bool sign0 = ((oldv0 ^ v1) & FLAG_NEGATIVE);	// 계산전 부호
	bool sign1 = ((v0 ^ v1) & FLAG_NEGATIVE);		// 계산후 부호

	// Overflow는 같은 부호를 더했는데 다른 부호가 나오면 Overflow이다
	//SetFlag(FLAG_OVERFLOW, (sign0 != sign1));
	Flag.V = (sign0 != sign1);
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
				// $0000 to $00FF
				// Zero page에서 읽어서 A로
				WORD addr = addr_mode_ZP(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ZPX : // 4 cycle
			{
				// Zero page의 주소와 X 레지스터를 더한 주소에서 읽어 A로..
				// X 레지스터에 $0F이고 LDA $80, X 이면 $80+$0F = $8F에서 A로 읽게됨
/*				
				BYTE zpa = Fetch(mem, cycle);
				zpa += X;
				cycle--;
*/
				WORD addr = addr_mode_ZPX(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			// 절대 주소 지정을 사용하는 명령어는 대상 위치를 식별하기 위해 전체 16 비트 주소를 포함합니다.
			case LDA_ABS: // 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSX:// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				// 메모리엑세스 페이지를 넘어가면 추가 사이클이 소요됨 (하드웨어가 그렇게 만들어짐?)
/*				
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);
				
				WORD t = lo + X;
				if( t > 0xFF ) cycle--;
				WORD addr = (lo | (hi << 8)) + X;
*/				
// 				WORD addr = FetchWord(mem, cycle);
// 				if ( (addr + X) - addr >= 0xFF )
// 					cycle--;	// page 넘어감

				WORD addr = addr_mode_ABSX(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSY:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
/*				
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);

				WORD t = lo + Y;
				if (t > 0xFF) cycle--;
				WORD addr = (lo | (hi << 8)) + Y;
*/
				WORD addr = addr_mode_ABSY(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);

			}
			break;

			case LDA_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case LDA_INDY: // 5 ~ 6 cycle
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A = ReadByte(mem, addr, cycle);

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
				WORD addr = addr_mode_ZP(mem, cycle);
 				X = ReadByte(mem, addr, cycle);
 				SetZeroNegative(X);
			}
			break;

			case LDX_ZPY:	// 4cycle
			{
/*				
				BYTE zpage = Fetch(mem, cycle);
				zpage += Y;
				cycle--;
*/
				WORD addr = addr_mode_ZPY(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);
			}
			break;

			case LDX_ABS:	// 4cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			case LDX_ABSY:	// 4 ~ 5 cycle
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case LDY_IM : // 2 cycle
			{
				Y = Fetch(mem, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZP: // 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZPX : // 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABS : // 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABSX : // 4~5 cycle
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STA_ZP	:	// 3 cycle
			{
				// ZeroPage에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ZPX :	// 4 cycle
			{
				// ZP + X에 A레지스터 내용쓰기
				WORD addr = addr_mode_ZPX(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABS:	// 4 cycle
			{
				// WORD address에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSX:	// 5 cycle
			{
				// WORD address + X에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSY:	// 5 cycle
			{
				// WORD address + Y에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABSY_NoPage(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
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
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			case STX_ZPY:	// 4 cycle
			{
				// ZP + Y에 X레지스터 내용쓰기
				WORD addr = addr_mode_ZPY(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			case STX_ABS:	// 4 cycle
			{
				// WORD address에 X레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STY_ZP:	// 3 cycle
			{
				// ZeroPage에 X레지스터 내용 쓰기
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, Y, addr, cycle);

			}
			break;

			case STY_ZPX:	// 4 cycle
			{
				// ZP + X에 Y레지스터 내용쓰기
				WORD addr = addr_mode_ZPX(mem, cycle);
				WriteByte(mem, Y, addr, cycle);
			}
			break;

			case STY_ABS:	// 4 cycle
			{
				// WORD address에 Y레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
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
				WORD addr = addr_mode_ABS(mem, cycle);
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

			// Pushes a copy of the status flags on to the stack.
			case PHP :	// 3 cycle
			{
				// PS -> Stack에 Push
				PushStackByte(mem, PS, cycle);
				cycle--;
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case AND_IM :	// 2 cycle
			{
				A &= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ZP:	// 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ZPX:	// 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABS:	// 4cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABSX:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABSY:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case AND_INDY:	// 5 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A &= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;


			case ORA_IM :
			{
				A |= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case ORA_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A |= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;


			case EOR_IM:
			{
				A ^= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case EOR_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			// Bit Test Zero page
			case BIT_ZP:
			{
				// Zp에서 읽은 값과 A를 & 테스트 하고 플레그들을 셋팅 / Set if the result if the AND is zero
				// N 플레그는 7bit, Set to bit 7 of the memory value
				// V 플레그는 6Bit , Set to bit 6 of the memory value
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE R = ReadByte(mem, addr, cycle);

				Flag.Z = !(A & R);	
				Flag.N = (R & FLAG_NEGATIVE) != 0;
				Flag.V = (R & FLAG_OVERFLOW) != 0;
			}
			break;

			case BIT_ABS :
			{
				// Zp에서 읽은 값과 A를 & 테스트 하고 플레그들을 셋팅
				// N 플레그는 7bit / V 플레그는 6Bit
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE R = ReadByte(mem, addr, cycle);

				Flag.Z = !(A & R);
				Flag.N = (R & FLAG_NEGATIVE) != 0;
				Flag.V = (R & FLAG_OVERFLOW) != 0;

			}
			break;

			////////////////////////////////////////////////////////////////////////////// Register Transfer

			case TAX :
			{
				// Transfer Accumulator to X
				X = A;
				cycle--;
				Flag.Z = (X == 0);
				Flag.N = (X & FLAG_NEGATIVE) != 0;
			}
			break;

			case TAY:
			{
				//Transfer Accumulator to Y
				Y = A;
				cycle--;
				Flag.Z = (Y == 0);
				Flag.N = (Y & FLAG_NEGATIVE) != 0;
			}
			break;

			case TXA:
			{
				// Transfer X to Accumulator
				A = X;
				cycle--;
				Flag.Z = (A == 0);
				Flag.N = (A & FLAG_NEGATIVE) != 0;
			}
			break;

			case TYA:
			{
				// Transfer Y to Accumulator
				A = Y;
				cycle--;
				Flag.Z = (A == 0);
				Flag.N = (A & FLAG_NEGATIVE) != 0;
			}
			break;


			////////////////////////////////////////////////////////////////////////////// increase / decrease

			case INX :
			{
				// Increment X Register / X,Z,N = X+1
				X++;
				cycle--;
				Flag.Z = (X == 0);
				Flag.N = (X & FLAG_NEGATIVE) != 0;
			}
			break;

			case INY :
			{
				// Increment Y Register / Y,Z,N = Y+1
				Y++;
				cycle--;
				Flag.Z = (Y == 0);
				Flag.N = (Y & FLAG_NEGATIVE) != 0;
			}
			break;
			case DEX:
			{
				// Decrease X Register / X,Z,N = X+1
				X--;
				cycle--;
				Flag.Z = (X == 0);
				Flag.N = (X & FLAG_NEGATIVE) != 0;

			}
			break;

			case DEY:
			{
				// Decrement Y Register / Y,Z,N = Y+1
				Y--;
				cycle--;
				Flag.Z = (Y == 0);
				Flag.N = (Y & FLAG_NEGATIVE) != 0;

			}
			break;

			case INC_ZP:
			{
				// Increment Memory by One / M + 1 -> M
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case INC_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case INC_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);				
			}
			break;
			case INC_ABSX:
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);				
			}
			break;
			case DEC_ZP:// 5 cycle
			{
				// Decrement Memory by One / M - 1 -> M
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);

			}
			break;
			case DEC_ZPX: // 6 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case DEC_ABS:	// 6 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;

			case DEC_ABSX:	// 7 cycle
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////	Arithmetic

			// Add with Carry
			// This instruction adds the contents of a memory location to the accumulator together with 
			// the carry bit.If overflow occurs the carry bit is set, this enables multiple byte addition to be performed.
			case ADC_IM	:	// 2 cycle
			{
				// A + M + C -> A, C
				BYTE v = Fetch(mem, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ZP:	// 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ZPX:	// 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABS:	// 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABSX:	// 4~5 cycle
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABSY:	// 4~5 cycle
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_INDY:	// 5~6 cycle
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// SBC - Subtract with Carry
			// A, Z, C, N = A - M - (1 - C)
			case SBC_IM	:
			{
				BYTE v = Fetch(mem, cycle);

			}
			break;

			case SBC_ZP:
			{
			}
			break;

			case SBC_ZPX:
			{
			}
			break;

			case SBC_ABS:
			{
			}
			break;

			case SBC_ABSX:
			{
			}
			break;

			case SBC_ABSY:
			{
			}
			break;

			case SBC_INDX:
			{
			}
			break;

			case SBC_INDY:
			{
			}
			break;

			////////////////////////////////////////////////////////////////////////////// Compare

			// Z,C,N = A-M
			// This instruction compares the contents of the accumulator with another memory held 
			// valueand sets the zeroand carry flags as appropriate.
			case CMP_IM:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;
			
			case CMP_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}

			break;

			case CMP_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// Compare X Register
			// Z, C, N = X - M
			// This instruction compares the contents of the X register with another memory
			// held value and sets the zero and carry flags as appropriate.
			case CPX_IM	:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CPX(v);
			}
			break;

			case CPX_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPX(v);
			}
			break;

			case CPX_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPX(v);
			}
			break;

			case CPY_IM:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CPY(v);
			}
			break;

			case CPY_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPY(v);
			}
			break;

			case CPY_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPY(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case NOP :
				cycle--;
			break;

			//////////////////////////////////////////////////////////////////////////////

			default:
				printf("Unknown instruction : %x\n", inst);
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

////////////////////////////////////////////////////////////////////////////// memory addressing mode

// ZeroPage
WORD  CPU::addr_mode_ZP(Memory &mem, int &cycle)
{
	BYTE address = Fetch(mem, cycle);
	return address;
}

// Zero page + X
WORD CPU::addr_mode_ZPX(Memory& mem, int& cycle)
{
	BYTE address = Fetch(mem, cycle) + X;
	cycle--;
	return address;
}

// Zero page + X
WORD CPU::addr_mode_ZPY(Memory& mem, int& cycle)
{
	BYTE address = Fetch(mem, cycle) + Y;
	cycle--;
	return address;
}

// ABS
WORD CPU::addr_mode_ABS(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	return address;
}

// ABS + X
WORD CPU::addr_mode_ABSX(Memory& mem, int& cycle)
{
	BYTE lo = Fetch(mem, cycle);
	BYTE hi = Fetch(mem, cycle);
	WORD t = lo + X;
	if (t > 0xFF) cycle--;
	WORD address = (lo | (hi << 8)) + X;
	return address;
}

// ABS + X : Page 넘어가는것 무시(그냥 하드웨어가 이렇게 생김)
WORD CPU::addr_mode_ABSX_NoPage(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	address += X;
	cycle--;
	return address;
}

// ABS + Y
WORD CPU::addr_mode_ABSY(Memory& mem, int& cycle)
{
	BYTE lo = Fetch(mem, cycle);
	BYTE hi = Fetch(mem, cycle);

	WORD t = lo + Y;
	if (t > 0xFF) cycle--;
	WORD address = (lo | (hi << 8)) + Y;
	return address;
}

WORD CPU::addr_mode_ABSY_NoPage(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	address += Y;
	cycle--;
	return address;
}

WORD CPU::addr_mode_INDX(Memory& mem, int& cycle)
{
	BYTE t = Fetch(mem, cycle);
	WORD inx = t + X;
	cycle--;
	WORD address = ReadWord(mem, inx, cycle);
	return address;
}


WORD CPU::addr_mode_INDY(Memory& mem, int& cycle)
{
#if 1
	// zero page에서 word 읽고 Y레지스터와 더한 주소의 1바이트를 A에 로드
	// 읽을 주소가 page를 넘으면 1사이클 감소
	BYTE addr = Fetch(mem, cycle);
	BYTE lo = ReadByte(mem, addr, cycle);
	BYTE hi = ReadByte(mem, addr + 1, cycle);

	WORD t = lo + Y;
	if (t > 0xFF) cycle--;	// page 넘어감

	WORD index_addr = ((hi << 8) | lo) + Y;
	return index_addr;
#else
	BYTE addr = Fetch(mem, cycle);
	WORD EAddr = ReadWord(mem, addr, cycle);
	WORD EAddrY = EAddr + Y;
	const bool CrossedPageBoundary = (EAddr ^ EAddrY) >> 8;
	if (CrossedPageBoundary)
	{
		cycle--;
	}
	return EAddrY;
#endif
}

void CPU::Execute_ADC(BYTE v)
{
	BYTE oldA = A;
	WORD Result = A + v + Flag.C;
	A = (Result & 0xFF);

	SetZeroNegative(A);
	SetCarryFlag(Result);
	SetOverflow(oldA, A, v);
}

void CPU::Execute_CMP(BYTE v)
{
	WORD t = A - v;
	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	Flag.Z = A == v;					// Set if A = M
	Flag.C = A >= v;					// Set if A >= M
}

void CPU::Execute_CPX(BYTE v)
{
	WORD t = X - v;
	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	Flag.Z = X == v;					// Set if X = M
	Flag.C = X >= v;					// Set if X >= M
}

void CPU::Execute_CPY(BYTE v)
{
	WORD t = Y - v;
	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	Flag.Z = Y == v;					// Set if Y = M
	Flag.C = Y >= v;					// Set if Y >= M
}
