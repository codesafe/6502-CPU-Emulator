#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"

struct SoftSwitch
{
	bool switched;
	WORD address;
	BYTE value;
	BYTE readorwrite;	// false : read , true : write

	void Reset()
	{
		switched = false;
		address = 0;
		value = 0;
		readorwrite = false;
	}

	void SetSwitch(WORD addr, BYTE v, bool rw)
	{
		switched = true;
		address = addr;
		value = v;
		readorwrite = rw;
	}
};

class Memory
{
	public:
		BYTE* memory;
		SoftSwitch softswitch;

	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();

		void CheckSoftSwitch(int addr, BYTE v, bool rw)
		{
			softswitch.Reset();

			if (addr == 0xCFFF || ((addr & 0xFF00) == 0xC000))
				softswitch.SetSwitch(addr, v, rw);
		}

// 		BYTE operator [] (int addr) const
// 		{
// 			return memory[addr];
// 		}
// 
// 		BYTE& operator[] (int addr)
// 		{
// 			return memory[addr];
// 		}

		BYTE GetByte(int addr)
		{
			BYTE v = memory[addr];
			CheckSoftSwitch(addr, v, false);
			return v;
		}

		void SetByte(int addr, BYTE value)
		{
			memory[addr] = value;
			CheckSoftSwitch(addr, value, true);
		}

		BYTE ReadByte(int addr)
		{
			return memory[addr];
		}

		WORD ReadWord(int addr)
		{
			BYTE m0 = memory[addr];
			BYTE m1 = memory[addr+1];
			WORD w = (m1 << 8) | m0;
			return w;
		}

		void WriteWord(WORD value, int addr)
		{
			memory[addr] = value >> 8;
			memory[addr+1] = value & 0xFF;
		}

		WORD UpLoadProgram(BYTE *code, int codesize);
		void UpLoadProgram(int startPos, BYTE *code, int codesize);
};


#endif