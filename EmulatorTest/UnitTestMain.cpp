
#include "UnitTestMain.h"

// 6502 CPU
CPU cpu;
// Memory
Memory mem;

// ZERO / NAGATIVE Flag를 뺸 나머지 플레그가 변경되었나?
void CheckCPUFlagModified(CPU &cpu0, CPU& cpu1)
{
	EXPECT_EQ(cpu0.Flag.C, cpu1.Flag.C);
	EXPECT_EQ(cpu0.Flag.I, cpu1.Flag.I);
	EXPECT_EQ(cpu0.Flag.D, cpu1.Flag.D);
	EXPECT_EQ(cpu0.Flag.B, cpu1.Flag.B);
	EXPECT_EQ(cpu0.Flag.V, cpu1.Flag.V);
}

// Zero / Negative Flag 검사
void CheckZeroOrNegative()
{
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}


int RunMachine(int cycle)
{
	return cpu.Run(mem, cycle);
}


GTEST_API_ int main(int argc, char** argv)
{
	cpu.Reset();
	mem.Create();

	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

