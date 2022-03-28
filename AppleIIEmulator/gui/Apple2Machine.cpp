
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

	// dirty hack, fix soon... if I understand why
	mem.WriteByte(0x4D, 0xAA);   // Joust crashes if this memory location equals zero
	mem.WriteByte(0xD0, 0xAA);   // Planetoids won't work if this memory location equals zero

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
#if 0
		fread(rom, ROMSIZE, 1, fp);
		mem.UpLoadProgram(ROMSTART, rom, ROMSIZE);
#else
		fread(mem.rom, ROMSIZE, 1, fp);
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
#if 0
		fread(disk2, SL6SIZE, 1, disk2fp);
		mem.UpLoadProgram(SL6START, disk2, SL6SIZE);
#else
		fread(mem.sl6, SL6SIZE, 1, disk2fp);
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