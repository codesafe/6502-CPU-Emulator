
#include "Apple2Machine.h"
#include "raylib.h"

Image backbuffer;
Texture2D texture;
Color* pixels;

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


	pixels = (Color*)malloc(400 * 400* sizeof(Color));

	for (int y = 0; y < 400; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			if (((x / 32 + y / 32) / 1) % 2 == 0) pixels[y * 100 + x] = ORANGE;
			else pixels[y * 100 + x] = GOLD;
		}
	}

	Image checkedIm;
	checkedIm.data = pixels;
	checkedIm.width = 400;
	checkedIm.height = 400;
	checkedIm.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	checkedIm.mipmaps = 1;

	texture = LoadTextureFromImage(checkedIm);
	//UnloadImage(checkedIm);

	//texture = LoadTextureFromImage(backbuffer);

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

static bool aa = false;
void UpdateTexture()
{
	for (int y = 0; y < 400; y++)
	{
		for (int x = 0; x < 400; x++)
		{
			if (((x / 32 + y / 32) / 1) % 2 == 0) pixels[y * 400 + x] = aa ? GREEN : VIOLET;
			else pixels[y * 400 + x] = aa ? RED : BLUE;
		}
	}

	Image checkedIm;
	checkedIm.data = pixels;
	checkedIm.width = 400;
	checkedIm.height = 400;
	checkedIm.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	checkedIm.mipmaps = 1;

	UnloadTexture(texture);
	texture = LoadTextureFromImage(checkedIm);
	aa = !aa;
}

void Apple2Machine::Render(int frame)
{
	device.Render(mem, frame);

// 	Vector2 pos;
// 	pos.x = 300;
// 	pos.y = 300;
// 	DrawTextureEx(texture, pos, 0, 1, WHITE);
// 	UpdateTexture();
}