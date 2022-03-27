
#include "Apple2Machine.h"
#include "raylib.h"

Apple2Machine::Apple2Machine()
{

}

Apple2Machine::~Apple2Machine()
{

}

void Apple2Machine::InitMachine()
{
	mem.Create();

	device.InsetFloppy();
	// unset the Power-UP byte
	mem.WriteByte(0x3F4, 0);

	cpu.Reset(mem);

	device.Create();
	mem.device = &device;

	UploadRom();
}

bool Apple2Machine::UploadRom()
{
	bool ret = false;

	// load the Apple II+ ROM
	BYTE* rom = new BYTE[ROMSIZE];
	FILE* fp = fopen("rom/appleII+.rom", "rb"); 
	if (fp)
	{
		fread(rom, ROMSIZE, 1, fp);
#if 0
		mem.UpLoadProgram(ROMSTART, rom, ROMSIZE);
#else
		memcpy(mem.rom, rom, ROMSIZE);
#endif
		fclose(fp);
		ret = true;
	}
	delete[] rom;

	// load Apple II+ / Disk II
	BYTE* disk2 = new BYTE[SL6SIZE];
	FILE* disk2fp = fopen("rom/diskII.rom", "rb");
	if (disk2fp)
	{
		fread(disk2, SL6SIZE, 1, disk2fp);
#if 0
		mem.UpLoadProgram(SL6START, disk2, SL6SIZE);
#else
		memcpy(mem.sl6, disk2, SL6SIZE);
#endif
		fclose(disk2fp);
		ret = true;
	}
	delete[] disk2;

	cpu.Reset(mem);
	return ret;
}

void Apple2Machine::Run(int cycle)
{
	device.UpdateKeyBoard();
	cpu.Run(mem, cycle);
	while(device.UpdateFloppyDisk())
		cpu.Run(mem, 5000);
}


void Apple2Machine::Render(int frame)
{
	device.Render(mem, frame);
}