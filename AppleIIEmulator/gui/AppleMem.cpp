
/*
	MOS 6502 CPU Emulator
*/

#include "AppleMem.h"

Memory::Memory()
{
	memory = NULL;
	device = NULL;
}

Memory::~Memory()
{

}

void Memory::Create()
{
	memory = new BYTE[MEMORY_SIZE];
	memset(memory, 0, MEMORY_SIZE);

	// Language Card writable
	LCWR = true;
	// Language Card readable
	LCRD = false;
	// Language Card bank 2 enabled
	LCBK2 = true;
	// Language Card pre-write flip flop
	LCWFF = false;
}

void Memory::Destroy()
{
	delete[] memory;
	memory = NULL;
}

BYTE Memory::ReadByte(int address)
{
#if 0
	BYTE v = memory[address];
	if (address == 0xCFFF || ((address & 0xFF00) == 0xC000))
		v = device->SoftSwitch(address, 0, false);
	return v;
#else
	if (address < RAMSIZE)
		return(ram[address]);                                                 // RAM

	if (address >= ROMSTART) 
	{
		if (!LCRD)
			return(rom[address - ROMSTART]);                                    // ROM

		if (LCBK2 && (address < 0xE000))
			return(bk2[address - BK2START]);                                    // BK2

		return(lgc[address - LGCSTART]);                                       // LC
	}

	if ((address & 0xFF00) == SL6START)
		return(sl6[address - SL6START]); // disk][

	if ((address & 0xF000) == 0xC000)
		return (device->SoftSwitch(this, address, 0, false));

	return 0;
#endif
}

void Memory::WriteByte(int address, BYTE value)
{
#if 0

	memory[address] = value;
	if (address == 0xCFFF || ((address & 0xFF00) == 0xC000))
		device->SoftSwitch(address, value, true);
#else
	if (address < RAMSIZE) 
	{
		ram[address] = value;                                                 // RAM
		return;
	}

	if (LCWR && (address >= ROMSTART)) 
	{
		if (LCBK2 && (address < 0xE000)) 
		{
			bk2[address - BK2START] = value;                                    // BK2
			return;
		}
		lgc[address - LGCSTART] = value;                                       // LC
		return;
	}

	if ((address & 0xF000) == 0xC000)
	{
		device->SoftSwitch(this, address, value, true);
		return;
	}
#endif
}

WORD Memory::ReadWord(int addr)
{
	BYTE m0 = ReadByte(addr);
	BYTE m1 = ReadByte(addr + 1);
	WORD w = (m1 << 8) | m0;
	return w;
}

void Memory::WriteWord(WORD value, int addr)
{
	WriteByte(addr, value >> 8);
	WriteByte(addr + 1, value & 0xFF);
}


// 프로그램 업로드
WORD Memory::UpLoadProgram(BYTE* code, int codesize)
{
	// 상위 1 WORD는 시작 위치 (PC)
	WORD addr = code[0] | (code[1] << 8);
	for (int i = 0; i < codesize-2; i++)
	{
		memory[addr+i] = code[i+2];
	}
	return addr;
}

void Memory::UpLoadProgram(int startPos, BYTE* code, int codesize)
{
	for (int i = 0; i < codesize; i++)
		memory[startPos+i] = code[i];
}


void Memory::UpLoadToRom(BYTE* code)
{
	memcpy(rom, code, ROMSIZE);
}