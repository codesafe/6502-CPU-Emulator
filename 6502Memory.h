#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"

class Memory
{
	private:
		BYTE* memory;


	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();

		BYTE operator [] (int addr) const
		{
			return memory[addr];
		}

		BYTE& operator[] (int addr)
		{
			return memory[addr];
		}

		BYTE GetByte(int addr) const
		{
			return memory[addr];
		}

		void SetByte(int addr, BYTE value)
		{
			memory[addr] = value;
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
			memory[addr] = value & 0xFF;
			memory[addr+1] = value >> 8;
		}
};


#endif