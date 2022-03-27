#ifndef PREDEF_H
#define PREDEF_H


/*
	MOS 6502 CPU Emulator
*/

#include <stdio.h>
#include <memory.h>

#define SBYTE		signed char
#define BYTE		unsigned char
#define WORD		unsigned short
#define MEMORY_SIZE		1024 * 64	// 64k

#define RAMSIZE			0xC000
#define ROMSIZE			0x3000
#define ROMSTART		0xD000
#define SL6START		0xC600
#define SL6SIZE			0x00FF

#define FONT_X				7
#define FONT_Y				8
#define FONT_NUM			128
#define FONT_NORMAL			0
#define FONT_INVERSE		1
#define FONT_FLASH			2

#define SCREENTEXT_X	40
#define SCREENTEXT_Y	24

#define SCREENSIZE_X	280
#define SCREENSIZE_Y	192

#define TARGET_FRAME	30

#define TEXT_MODE		0
#define LORES_MODE		1
#define LORES_MIX_MODE	2
#define HIRES_MODE		3
#define HIRES_MIX_MODE	4

#endif