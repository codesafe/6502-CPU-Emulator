#include <stdio.h>
#include "UnitTestMain.h"

TEST(TEST_LOADASSEMCODE, ASSEMCODETEST)
{
	constexpr int codesize = 65536;
	BYTE* buffer = new BYTE[codesize];

	FILE* fp;
	fopen_s(&fp,"6502_functional_test.bin", "rb");
	fread(buffer, 1, codesize, fp);
	fclose(fp);

	mem.UpLoadProgram(0x0000, buffer, codesize);
	delete[] buffer;
	//cpu.Reset(mem);
	cpu.PC = 0x400;

	long count = 0;
	while (true)
	{
		cpu.Run(mem, 1);
		if( cpu.PC == 0x3469)
			break;
// 		if (count > 100000)
// 			break;
		count++;
	}

}
