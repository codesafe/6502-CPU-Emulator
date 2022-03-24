#ifndef APPLE2_DEVICE_H
#define APPLE2_DEVICE_H

#include <stdio.h>

#define RAMSIZE  0xC000
#define ROMSIZE  0x3000
#define ROMSTART 0xD000

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

public:
	CPU cpu;
	Memory mem;

private:
	BYTE ExecSoftSwitch(WORD address, BYTE value, bool WRT);

public:
	Apple2Device();
	~Apple2Device();

	void InitDevice();
	bool UploadRom();

	void SoftSwitch();

	void playSound();
	void Render();

};



#endif
