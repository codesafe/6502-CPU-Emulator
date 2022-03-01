/*
	MOS 6502 CPU Emulator
*/

#include "6502CPU.h"
#include "6502Memory.h"
#include "TestLoad.h"


// 6502 CPU
CPU cpu;
// Memory
Memory mem;

//////////////////////////////////////////////////////////////////////////////

// TEST Code
TEST_LOAD testload;

//////////////////////////////////////////////////////////////////////////////


void RunMachine(int cycle)
{
	cpu.Run(mem, cycle);
}


int main()
{
	cpu.Reset();
	mem.Create();

	int cycle = testload.DOTEST(LDX_ZP);

	RunMachine(cycle);

	mem.Destroy();
	return 0;
}
