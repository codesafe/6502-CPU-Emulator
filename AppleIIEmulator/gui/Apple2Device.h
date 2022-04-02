#ifndef APPLE2_DEVICE_H
#define APPLE2_DEVICE_H

#include <stdio.h>
#include <string>
#include "AppleFont.h"
#include "SDL_joystick.h"

class CPU;	// 6502 cpu
class Memory;

struct _RECT
{
	int x, y, width, height;
};

// two disk ][ drive units
struct FloppyDrive
{
	// the full disk image pathname
	char filename[400];
	// based on the image file attributes
	bool readOnly;
	// nibblelized disk image
	BYTE data[232960];
	// motor status
	bool motorOn;
	// writes to file are not implemented
	bool writeMode;
	// current track position
	BYTE track;
	// ptr to nibble under head position
	WORD nibble;
};


struct GamePad
{
	SDL_Joystick* controller;
	bool isavailable;
	bool axis[4];
	bool pressbtn1;
	bool pressbtn2;

	GamePad()
	{
		controller = NULL;
		isavailable = false;
		axis[0] = axis[1] = axis[2] = axis[3] = false;
		pressbtn1 = false;
		pressbtn2 = false;
	}
};

// apple2의 cpu / memory제외한 device
class Apple2Device
{
public:

	bool resetMachine;
	bool colorMonitor;
	BYTE zoomscale;

	//////////////////////////////////////////////////////////////////////////

	// 패들정보
	float GCP[2]; // GC Position ranging from 0 (left) to 255 right
	float GCC[2]; // $C064 (GC0) and $C065 (GC1) Countdowns
	int GCD[2];// GC0 and GC1 Directions (left/down or right/up)
	int GCA[2];// GC0 and GC1 Action (push or release)
	BYTE GCActionSpeed; // Game Controller speed at which it goes to the edges
	BYTE GCReleaseSpeed;// Game Controller speed at which it returns to center
	long long int GCCrigger; // $C070 the tick at which the GCs were reseted

	GamePad gamepad;

	//////////////////////////////////////////////////////////////////////////

	// 현재 플로피 디스크 (1,2)
	int	currentDrive;

	bool textMode;
	bool mixedMode;
	bool hires_Mode;
	BYTE videoPage;
	WORD videoAddress;

	_RECT pixelGR;

private:
	CPU* cpu;
	Color* backbuffer;	// Render Backbuffer
	Texture2D renderTexture;
	Image renderImage;

	AppleFont font;

	// 키보드입력 값
	BYTE keyboard;

	////////////////////////////////////////////////

	FloppyDrive disk[2];
	BYTE updatedrive;

	bool phases[2][4];
	// phases states Before
	bool phasesB[2][4];
	// phases states Before Before
	bool phasesBB[2][4];
	// phase index (for both drives)
	int pIdx[2];
	// phase index Before
	int pIdxB[2];
	int halfTrackPos[2];
	BYTE dLatch;

	////////////////////////////////////////////////

	short audioBuffer[2][AUDIOBUFFERSIZE];
	unsigned int audioDevice;

	bool silence;
	int volume;
	bool speaker;
	long long int speakerLastTick;

	void resetPaddles();
	BYTE readPaddle(int pdl);

	// DISK2
	bool InsertFloppy(const char* filename, int drv);
	void stepMotor(WORD address);
	void setDrv(int drv);

	// Keyboard
	void UpdateKeyBoard();
	// GamePad
	void UpdateGamepad();

	void ClearScreen();
	void DrawPoint(int x, int y, int r, int g, int b);
	void DrawRect(_RECT rect, int r, int g, int b);
	int GetScreenMode();

public:
	Apple2Device();
	~Apple2Device();

	void Create(CPU* cpu);
	void Dump(FILE* fp);
	void LoadDump(FILE* fp);

	BYTE SoftSwitch(Memory* mem, WORD address, BYTE value, bool WRT);
	void PlaySound();
	void Render(Memory& mem, int frame);

	void UpdateInput();

	bool UpdateFloppyDisk();
	void InsetFloppy();

	void FileDroped(char* path);
	bool GetDiskMotorState();
	std::string GetDiskName(int i);
};



#endif
