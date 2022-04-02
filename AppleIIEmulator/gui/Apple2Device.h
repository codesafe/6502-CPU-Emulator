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

// apple2�� cpu / memory������ device
class Apple2Device
{
public:

	//AudioStream stream;

	bool resetMachine;
	bool colorMonitor;
	BYTE zoomscale;

	//////////////////////////////////////////////////////////////////////////

	// �е�����
	BYTE PB0;  // $C061 Push Button 0 (bit 7) / Open Apple
	BYTE PB1;  // $C062 Push Button 1 (bit 7) / Solid Apple
	BYTE PB2;  // $C063 Push Button 2 (bit 7) / shift mod !!!
	float GCP[2]; // GC Position ranging from 0 (left) to 255 right
	float GCC[2]; // $C064 (GC0) and $C065 (GC1) Countdowns
	int GCD[2];// GC0 and GC1 Directions (left/down or right/up)
	int GCA[2];// GC0 and GC1 Action (push or release)
	BYTE GCActionSpeed; // Game Controller speed at which it goes to the edges
	BYTE GCReleaseSpeed;// Game Controller speed at which it returns to center
	long long int GCCrigger; // $C070 the tick at which the GCs were reseted

	GamePad gamepad;

	//////////////////////////////////////////////////////////////////////////


	// ���� �÷��� ��ũ (1,2)
	int	currentDrive;

	// $C050 CLRTEXT / $C051 SETTEXT
	bool textMode;

	// $C052 CLRMIXED / $C053 SETMIXED
	bool mixedMode;

	// ���� ������
	BYTE videoPage;
	// ���ػ� ���
	bool hires_Mode;

	// Ű�����Է� ��
	BYTE keyboard;
	// ���� �޸� �ּ�
	WORD videoAddress;

	_RECT pixelGR;

private:
	CPU* cpu;
	Color* backbuffer;	// Render Backbuffer
	Texture2D renderTexture;
	Image renderImage;

	AppleFont font;
	FloppyDrive disk[2];
	BYTE updatedrive;

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
