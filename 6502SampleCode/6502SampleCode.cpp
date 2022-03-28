/*
#include <cstdio>
#include <stdio.h>
#include "m6502.h"

int main()
{

	m6502::Mem mem;
	m6502::CPU cpu;

	cpu.Reset(mem);

	FILE* fp;
	fopen_s(&fp, "6502_functional_test.bin", "rb");
	fread(&mem[0x000A], 1, 65526, fp);
	fclose(fp);

	cpu.PC = 0x400;
	while (true)
	{
		cpu.Execute(1, mem);
	}

	return 0;
}

*/
