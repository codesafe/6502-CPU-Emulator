#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"
#include "Apple2Device.h"

/*
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
*/


class Memory
{
	public:
		BYTE* memory;
		Apple2Device* device;

	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();

// 		BYTE operator [] (int addr) const
// 		{
// 			return memory[addr];
// 		}
// 
// 		BYTE& operator[] (int addr)
// 		{
// 			return memory[addr];
// 		}

		BYTE ReadByte(int addr)
		{
			BYTE v = memory[addr];
			if (addr == 0xCFFF || ((addr & 0xFF00) == 0xC000))
				v = device->SoftSwitch(addr, v, false);
			return v;
		}

		void WriteByte(int addr, BYTE value)
		{
			memory[addr] = value;
			if (addr == 0xCFFF || ((addr & 0xFF00) == 0xC000))
				device->SoftSwitch(addr, value, true);
		}

// 		BYTE ReadByte(int addr)
// 		{
// 			return memory[addr];
// 		}

		WORD ReadWord(int addr)
		{
			BYTE m0 = ReadByte(addr);
			BYTE m1 = ReadByte(addr+1);
			WORD w = (m1 << 8) | m0;
			return w;
		}

		void WriteWord(WORD value, int addr)
		{
			WriteByte(addr,value >> 8);
			WriteByte(addr+1, value & 0xFF);
		}

		WORD UpLoadProgram(BYTE *code, int codesize);
		void UpLoadProgram(int startPos, BYTE *code, int codesize);
};


#endif