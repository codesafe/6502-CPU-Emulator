
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
#if 0
	memory = new BYTE[MEMORY_SIZE];
	memset(memory, 0, MEMORY_SIZE);
#else
	LCWritable = true;
	LCReadable = false;
	LCBank2Enable = true;
	LCPreWriteFlipflop = false;
#endif
}

void Memory::Destroy()
{
#if 0
	delete[] memory;
	memory = NULL;
#endif
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
		return ram[address];                                                        // RAM

	if (address >= ROMSTART) {
		if (!LCReadable)
			return rom[address - ROMSTART];                                           // ROM

		if (LCBank2Enable && (address < 0xE000))
			return bk2[address - BK2START];                                           // BK2

		return lgc[address - LGCSTART];                                             // LC
	}

	if ((address & 0xFF00) == SL6START)
		return sl6[address - SL6START];  // disk][

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
	if (address < RAMSIZE) {
		ram[address] = value;                                                       // RAM
		return;
	}

	if (LCWritable && (address >= ROMSTART)) {
		if (LCBank2Enable && (address < 0xE000)) {
			bk2[address - BK2START] = value;                                          // BK2
			return;
		}
		lgc[address - LGCSTART] = value;                                            // LC
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

void Memory::ResetRam()
{
	memset(ram, 0, sizeof(ram));
}

void Memory::Dump(FILE* fp)
{
	fwrite(&LCWritable, 1, sizeof(bool), fp);
	fwrite(&LCReadable, 1, sizeof(bool), fp);
	fwrite(&LCBank2Enable, 1, sizeof(bool), fp);
	fwrite(&LCPreWriteFlipflop, 1, sizeof(bool), fp);

	fwrite(ram, 1, RAMSIZE, fp);
	fwrite(rom, 1, ROMSIZE, fp);
	fwrite(lgc, 1, LGCSIZE, fp);
	fwrite(bk2, 1, BK2SIZE, fp);
	fwrite(sl6, 1, SL6SIZE, fp);
}

void Memory::LoadDump(FILE* fp)
{
	fread(&LCWritable, 1, sizeof(bool), fp);
	fread(&LCReadable, 1, sizeof(bool), fp);
	fread(&LCBank2Enable, 1, sizeof(bool), fp);
	fread(&LCPreWriteFlipflop, 1, sizeof(bool), fp);

	fread(ram, 1, RAMSIZE, fp);
	fread(rom, 1, ROMSIZE, fp);
	fread(lgc, 1, LGCSIZE, fp);
	fread(bk2, 1, BK2SIZE, fp);
	fread(sl6, 1, SL6SIZE, fp);
}