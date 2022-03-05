#pragma once


#include "gtest/gtest.h"
#include "../6502CPU.h"
#include "../6502Memory.h"

// 6502 CPU
extern CPU cpu;
// Memory
extern Memory mem;

extern void CheckCPUFlagModified(CPU& cpu0, CPU& cpu1);
extern void CheckZeroOrNegative();
extern int RunMachine(int cycle);