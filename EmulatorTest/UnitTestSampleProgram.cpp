#include "UnitTestMain.h"


// ASM 프로그램 테스트
TEST(TEST_PROGRAM, SAMPLE_ASM_PROGRAM1)
{
	cpu.Reset();

/*
	* = $1000
	lda #$FF

	start
	sta $90
	sta $8000
	eor #$CC
	jmp start
*/

	BYTE code[] = { 0x00, 0x10, 0xA9, 0xFF, 0x85, 0x90, 0x8D, 0x00, 0x80, 0x49, 0xCC, 0x4C, 0x02, 0x10 };
	WORD addr = mem.UpLoadProgram(code, 14);
	cpu.SetPCAddress(addr);

	for (int clock = 100; clock > 0;)
	{
		clock -= RunMachine(1);
		printf("%x : %x : %x : %x : %x : %d\n", cpu.A, cpu.Flag.N, cpu.Flag.V, cpu.Flag.Z, cpu.PC, clock);
	}


}

