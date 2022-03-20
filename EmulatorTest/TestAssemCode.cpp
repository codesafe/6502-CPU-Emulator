#include <stdio.h>
#include "UnitTestMain.h"

TEST(TEST_LOADASSEMCODE, ASSEMCODETEST)
{
	constexpr int codesize = 65526;
	BYTE* buffer = new BYTE[codesize];

	FILE* fp;
	fopen_s(&fp,"6502_functional_test.bin", "rb");
	fread(buffer, 1, codesize, fp);
	fclose(fp);

	mem.UpLoadProgram(0x000A, buffer, codesize);
	delete[] buffer;

	cpu.PC = 0x400;

	while (true)
	{
		cpu.Run(mem, 1);
	}
}
