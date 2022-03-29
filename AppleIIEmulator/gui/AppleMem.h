#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"
#include "Apple2Device.h"

class Memory
{
	public:

		// Language Card writable
		bool LCWR;
		// Language Card readable
		bool LCRD;
		// Language Card bank 2 enabled
		bool LCBK2;
		// Language Card pre-write flip flop
		bool LCWFF;

		BYTE ram[RAMSIZE];  // 48K of ram in $000-$BFFF
		BYTE rom[ROMSIZE];  // 12K of rom in $D000-$FFFF
		BYTE lgc[LGCSIZE];  // Language Card 12K in $D000-$FFFF
		BYTE bk2[BK2SIZE];  // bank 2 of Language Card 4K in $D000-$DFFF
		BYTE sl6[SL6SIZE];  // P5A disk ][ PROM in slot 6

		BYTE* memory;
		Apple2Device* device;

	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();

		BYTE ReadByte(int addr);
		void WriteByte(int addr, BYTE value);
		WORD ReadWord(int addr);
		void WriteWord(WORD value, int addr);

		WORD UpLoadProgram(BYTE *code, int codesize);
		void UpLoadProgram(int startPos, BYTE *code, int codesize);

		void UpLoadToRom(BYTE* code);
		void ResetRam();
};


#endif