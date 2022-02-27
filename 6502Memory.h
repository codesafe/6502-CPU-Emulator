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

		BYTE GetByte(int addr) const
		{
			return memory[addr];
		}

		void SetByte(int addr, BYTE value)
		{
			memory[addr] = value;
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

		WORD ReadWord(int addr)
		{
			BYTE m0 = memory[addr];
			BYTE m1 = memory[addr+1];
			WORD w = (m0 >> 8) + m1;
			return w;
		}

		void WriteWord(WORD value, int addr)
		{
			memory[addr] = value & 0xFF;
			memory[addr+1] = value >> 8;
		}
};


#endif