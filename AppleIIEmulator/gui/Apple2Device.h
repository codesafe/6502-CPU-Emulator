#ifndef APPLE2_DEVICE_H
#define APPLE2_DEVICE_H

#include <stdio.h>
#include "AppleFont.h"

class CPU;	// 6502 cpu
class Memory;


// two disk ][ drive units
struct drive
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

// apple2의 cpu / memory제외한 device
class Apple2Device
{
public:
	// Current Drive - only one can be enabled at a time
	int	currentDrive;
	// $C050 CLRTEXT / $C051 SETTEXT
	bool textMode;
	// $C052 CLRMIXED / $C053 SETMIXED
	bool mixedMode;
	// $C054 PAGE1 / $C055 PAGE2
	WORD videoPage;
	// $C056 GR / $C057 HGR
	bool hires_Mode;
	// $C000, $C010 ascii value of keyboard input
	BYTE keyboard;
	// can be $400, $800, $2000 or $4000
	WORD videoAddress;

private:
	Color* backbuffer;	// Render Backbuffer
	Texture2D renderTexture;
	Image renderImage;

	AppleFont font;
	drive disk[2];

	// DISK2
	int insertFloppy(const char* filename, int drv);
	void stepMotor(WORD address);
	void setDrv(int drv);

	void ClearScreen();
	void DrawPoint(int x, int y);
	int GetScreenMode();

public:
	Apple2Device();
	~Apple2Device();

	void Create();
	BYTE SoftSwitch(Memory* mem, WORD address, BYTE value, bool WRT);
	void PlaySound();
	void Render(Memory& mem, int frame);

	void UpdateKeyBoard();
	bool UpdateFloppyDisk();
	void InsetFloppy();

	bool GetDiskMotorState();
};



#endif
