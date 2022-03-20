
/*
	MOS 6502 CPU Emulator
*/

#include "6502Memory.h"

Memory::Memory()
{
	memory = NULL;
}

Memory::~Memory()
{

}

void Memory::Create()
{
	memory = new BYTE[MEMORY_SIZE];
	memset(memory, 0, MEMORY_SIZE);
}

void Memory::Destroy()
{
	delete[] memory;
	memory = NULL;
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