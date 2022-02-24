
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
}

void Memory::Destroy()
{
	delete[] memory;
	memory = NULL;
}

BYTE Memory::Fetch()
{

}