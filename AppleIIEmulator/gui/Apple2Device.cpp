

#include "Predef.h"
#include "AppleCPU.h"
#include "AppleMem.h"
#include "Apple2device.h"
#include "AppleFont.h"
#include "raylib.h"


BYTE PB0 = 0; // $C061 Push Button 0 (bit 7) / Open Apple
BYTE PB1 = 0; // $C062 Push Button 1 (bit 7) / Solid Apple
BYTE PB2 = 0; // $C063 Push Button 2 (bit 7) / shift mod !!!

// GC Position ranging from 0 (left) to 255 right
float GCP[2] = { 127, 127 };
// $C064 (GC0) and $C065 (GC1) Countdowns
float GCC[2] = { 0 };
// GC0 and GC1 Directions (left/down or right/up)
int   GCD[2] = { 0 };
// GC0 and GC1 Action (push or release)
int   GCA[2] = { 0 };
// Game Controller speed at which it goes to the edges
BYTE GCActionSpeed = 8;
// Game Controller speed at which it returns to center
BYTE GCReleaseSpeed = 8;
// $C070 the tick at which the GCs were reseted
long long int GCCrigger;


Apple2Device::Apple2Device()
{
}

Apple2Device::~Apple2Device()
{

}

void Apple2Device::Create()
{
	font.Create();

	currentDrive = 0;

	textMode = true;
	mixedMode = false;
	videoPage = 1;
	hires_Mode = false;
	keyboard = 0;
	videoAddress = videoPage * 0x0400;
}


BYTE Apple2Device::SoftSwitch(Memory *mem, WORD address, BYTE value, bool WRT)
{
	// disk ][ I/O register
	static BYTE dLatch = 0;

	switch (address) 
	{
		// KEYBOARD
		case 0xC000: 
			return(keyboard);
		// KBDSTROBE
		case 0xC010: 
			keyboard &= 0x7F; 
			return(keyboard);

		case 0xC020: // TAPEOUT (shall we listen it ? - try SAVE from applesoft)
		case 0xC030: // SPEAKER
		case 0xC033: 
			PlaySound(); 
			break; // apple invader uses $C033 to output sound !

		///////////////////////////////////////////////////////////////////////////////// Graphics

		case 0xC050: 
			textMode = false; 
			break;
		// Text
		case 0xC051: 
			textMode = true;  
			break;

		// Mixed off
		case 0xC052: 
			mixedMode = false; 
			break;

		// Mixed on
		case 0xC053: 
			mixedMode = true;  
			break;
		// Page 1
		case 0xC054: 
			videoPage = 1;
			break;
		// Page 2
		case 0xC055: 
			videoPage = 2;
			break;

		// HiRes off
		case 0xC056: 
			hires_Mode = false; 
			break;
		// HiRes on
		case 0xC057: 
			hires_Mode = true;  
			break;

		/////////////////////////////////////////////////////////////////////////////////	Joy Paddle ?

// 		case 0xC061: return(PB0);                                                   // Push Button 0
// 		case 0xC062: return(PB1);                                                   // Push Button 1
// 		case 0xC063: return(PB2);                                                   // Push Button 2
// 		case 0xC064: return(readPaddle(0));                                         // Paddle 0
// 		case 0xC065: return(readPaddle(1));                                         // Paddle 1
// 		case 0xC066: return(readPaddle(0));                                         // Paddle 2 -- not implemented
// 		case 0xC067: return(readPaddle(1));                                         // Paddle 3 -- not implemented
//		case 0xC070: resetPaddles(); break;                                         // paddle timer RST

		/////////////////////////////////////////////////////////////////////////////////	DISK 2

		case 0xC0E0:
		case 0xC0E1:
		case 0xC0E2:
		case 0xC0E3:
		case 0xC0E4:
		case 0xC0E5:
		case 0xC0E6:
		case 0xC0E7: stepMotor(address); break;                                     // MOVE DRIVE HEAD

		// MOTOR OFF
		case 0xCFFF:
		case 0xC0E8: 
			disk[currentDrive].motorOn = false; 
			//printf("--> DISK MOTOR OFF\n");
			break;

		// MOTOR ON
		case 0xC0E9: 
			disk[currentDrive].motorOn = true;  
			//printf("--> DISK MOTOR ON\n");
			break;

		// DRIVE 0
		case 0xC0EA: 
			setDrv(0); 
			break;
		// DRIVE 1
		case 0xC0EB: 
			setDrv(1); 
			break;

		// Shift Data Latch
		case 0xC0EC:                                                                
		{
			// writting
			if (disk[currentDrive].writeMode)
				disk[currentDrive].data[disk[currentDrive].track * 0x1A00 + disk[currentDrive].nibble] = dLatch;
			else
			{
				// reading
				dLatch = disk[currentDrive].data[disk[currentDrive].track * 0x1A00 + disk[currentDrive].nibble];
			}

			// turn floppy of 1 nibble
			disk[currentDrive].nibble = (disk[currentDrive].nibble + 1) % 0x1A00;                 
		}
		return(dLatch);

		// Load Data Latch
		case 0xC0ED: 
			dLatch = value; 
			break;

		// latch for READ
		case 0xC0EE:
			disk[currentDrive].writeMode = false;
			return(disk[currentDrive].readOnly ? 0x80 : 0);                                 // check protection

		// latch for WRITE
		case 0xC0EF: 
			disk[currentDrive].writeMode = true; 
			break;

		///////////////////////////////////////////////////////////////////////////////// LANGUAGE CARD

		case 0xC080:                                                                // LANGUAGE CARD :
		case 0xC084: 
			mem->LCBK2 = 1; mem->LCRD = 1; mem->LCWR = 0;
			mem->LCWFF = 0;    
			break;       // LC2RD

		case 0xC081:
		case 0xC085: 
			mem->LCBK2 = 1; mem->LCRD = 0; mem->LCWR |= mem->LCWFF; 
			mem->LCWFF = !WRT; 
			break;       // LC2WR

		case 0xC082:
		case 0xC086: 
			mem->LCBK2 = 1; mem->LCRD = 0; mem->LCWR = 0;
			mem->LCWFF = 0;    
			break;       // ROMONLY2

		case 0xC083:
		case 0xC087: 
			mem->LCBK2 = 1; mem->LCRD = 1; mem->LCWR |= mem->LCWFF; 
			mem->LCWFF = !WRT; 
			break;       // LC2RW

		case 0xC088:
		case 0xC08C: 
			mem->LCBK2 = 0; mem->LCRD = 1; mem->LCWR = 0;
			mem->LCWFF = 0;    
			break;       // LC1RD

		case 0xC089:
		case 0xC08D: 
			mem->LCBK2 = 0; mem->LCRD = 0; mem->LCWR |= mem->LCWFF; 
			mem->LCWFF = !WRT; break;       // LC1WR

		case 0xC08A:
		case 0xC08E: 
			mem->LCBK2 = 0; mem->LCRD = 0; mem->LCWR = 0; 
			mem->LCWFF = 0;    break;       // ROMONLY1

		case 0xC08B:
		case 0xC08F: 
			mem->LCBK2 = 0; mem->LCRD = 1; 
			mem->LCWR |= mem->LCWFF; 
			mem->LCWFF = !WRT; 
			break;       // LC1RW
	}
	static int ticks = 0;
	ticks++;
	return (ticks % 256);
}



void Apple2Device::PlaySound()
{
	static long long int lastTick = 0LL;
	static bool SPKR = false;                                                     // $C030 Speaker toggle

// 	if (!muted) 
// 	{
// 		SPKR = !SPKR;                                                               // toggle speaker state
// 		Uint32 length = (ticks - lastTick) / 10.65625;                              // 1023000Hz / 96000Hz = 10.65625
// 		lastTick = ticks;
// 		if (length > audioBufferSize) length = audioBufferSize;
// 		SDL_QueueAudio(audioDevice, audioBuffer[SPKR], length | 1);                 // | 1 TO HEAR HIGH FREQ SOUNDS
// 	}
}

const int offsetGR[24] = {                                                    // helper for TEXT and GR video generation
  0x000, 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380,                     // lines 0-7
  0x028, 0x0A8, 0x128, 0x1A8, 0x228, 0x2A8, 0x328, 0x3A8,                     // lines 8-15
  0x050, 0x0D0, 0x150, 0x1D0, 0x250, 0x2D0, 0x350, 0x3D0 };                    // lines 16-23

int textPosx = 300;
int textPosy = 50;

void Apple2Device::Render(Memory &mem, int frame)
{
	int maxcolumn = 40;
	int maxline = 24;

	// video Page에 따라 Address가 달라짐
	// $400, $800, $2000, $4000
	videoAddress = videoPage * 0x0400;

	if (textMode == true || mixedMode == true)
	{
		// Text or Mixed
		// Font 크기 7X8 / 40x20 글자
		int linelimit = textMode ? 0 : 20;

		for (int col = 0; col < maxcolumn; col++)
		{
			for (int line = linelimit; line < maxline; line++)
			{
				// read video memory
				BYTE glyph = mem.ReadByte(videoAddress + offsetGR[line] + col);

				int fontattr = 0;
				if (glyph > 0x7F)
					fontattr = FONT_NORMAL;
				else if (glyph < 0x40) 
					fontattr = FONT_INVERSE;
				else 
					fontattr = FONT_FLASH;

				glyph &= 0x7F; // unset bit 7
				if (glyph > 0x5F) glyph &= 0x3F; // shifts to match
				if (glyph < 0x20) glyph |= 0x40; // the ASCII codes

				if (fontattr == FONT_NORMAL || (fontattr == FONT_FLASH && frame < 15))
				{
					font.RenderFont(glyph, textPosx + (col * 7), textPosy + (line * 8), false);
				}
				else
				{
					font.RenderFont(glyph, textPosx + (col * 7), textPosy + (line * 8), true);
				}
			}
		}
	}
	else
	{
		// LoRes 저해상도
		if (hires_Mode == false)
		{

		}
		else
		{
			// highRes 고해상도


		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Apple Disk II 

int Apple2Device::insertFloppy(const char* filename, int drv)
{
	FILE* f = fopen(filename, "rb");                                              // open file in read binary mode
	if (!f || fread(disk[drv].data, 1, 232960, f) != 232960)                      // load it into memory and check size
		return(0);
	fclose(f);

	sprintf(disk[drv].filename, "%s", filename);                                   // update disk filename record

// 	f = fopen(filename, "ab");                                                    // try to open the file in append binary mode
// 	if (!f) {                                                                      // success, file is writable
// 		disk[drv].readOnly = true;                                                  // update the readOnly flag
// 		fclose(f);                                                                  // and close it untouched
// 	}
// 	else disk[drv].readOnly = false;                                              // f is NULL, no writable, no need to close it

	return(1);
}


void Apple2Device::stepMotor(WORD address)
{
	static bool phases[2][4] = { 0 };                                             // phases states (for both drives)
	static bool phasesB[2][4] = { 0 };                                             // phases states Before
	static bool phasesBB[2][4] = { 0 };                                             // phases states Before Before
	static int pIdx[2] = { 0 };                                             // phase index (for both drives)
	static int pIdxB[2] = { 0 };                                             // phase index Before
	static int halfTrackPos[2] = { 0 };

	address &= 7;
	int phase = address >> 1;

	phasesBB[currentDrive][pIdxB[currentDrive]] = phasesB[currentDrive][pIdxB[currentDrive]];
	phasesB[currentDrive][pIdx[currentDrive]] = phases[currentDrive][pIdx[currentDrive]];
	pIdxB[currentDrive] = pIdx[currentDrive];
	pIdx[currentDrive] = phase;

	if (!(address & 1)) {                                                          // head not moving (PHASE x OFF)
		phases[currentDrive][phase] = false;
		return;
	}

	if ((phasesBB[currentDrive][(phase + 1) & 3]) && (--halfTrackPos[currentDrive] < 0))      // head is moving in
		halfTrackPos[currentDrive] = 0;

	if ((phasesBB[currentDrive][(phase - 1) & 3]) && (++halfTrackPos[currentDrive] > 140))    // head is moving out
		halfTrackPos[currentDrive] = 140;

	phases[currentDrive][phase] = true;                                                 // update track#
	disk[currentDrive].track = (halfTrackPos[currentDrive] + 1) / 2;
	disk[currentDrive].nibble = 0;                                                      // not sure this is necessary ?
}

void Apple2Device::setDrv(int drv)
{
	disk[drv].motorOn = disk[!drv].motorOn || disk[drv].motorOn;                  // if any of the motors were ON
	disk[!drv].motorOn = false;                                                   // motor of the other drive is set to OFF
	currentDrive = drv;                                                                 // set the current drive
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////// 키보드

void Apple2Device::UpdateKeyBoard()
{
	int key = GetKeyPressed();
	bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

	switch (key)
	{
// 		case KEY_A:            keyboard = 0xC1;   break;
// 		case KEY_B:            keyboard = 0xC2;   break;
// 		case KEY_C:            keyboard = 0xC3;   break;
// 		case KEY_D:            keyboard = 0xC4;   break;
// 		case KEY_E:            keyboard = 0xC5;   break;
// 		case KEY_F:            keyboard = 0xC6;   break;
// 		case KEY_G:            keyboard = 0xC7;   break;
// 		case KEY_H:            keyboard = 0xC8;   break;
// 		case KEY_I:            keyboard = 0xC9;   break;
// 		case KEY_J:            keyboard = 0xCA;   break;
// 		case KEY_K:            keyboard = 0xCB;   break;
// 		case KEY_L:            keyboard = 0xCC;   break;
// 		case KEY_M:            keyboard = 0xCD;   break;
// 		case KEY_N:            keyboard = 0xCE;   break;
// 		case KEY_O:            keyboard = 0xCF;   break;
// 		case KEY_P:            keyboard = 0xD0;   break;
// 		case KEY_Q:            keyboard = 0xD1;   break;
// 		case KEY_R:            keyboard = 0xD2;   break;
// 		case KEY_S:            keyboard = 0xD3;   break;
// 		case KEY_T:            keyboard = 0xD4;   break;
// 		case KEY_U:            keyboard = 0xD5;   break;
// 		case KEY_V:            keyboard = 0xD6;   break;
// 		case KEY_W:            keyboard = 0xD7;   break;
// 		case KEY_X:            keyboard = 0xD8;   break;
// 		case KEY_Y:            keyboard = 0xD9;   break;
// 		case KEY_Z:            keyboard = 0xDA;   break;

		case KEY_A:
		case KEY_B:
		case KEY_C:
		case KEY_D:
		case KEY_E:
		case KEY_F:
		case KEY_G:
		case KEY_H:
		case KEY_I:
		case KEY_J:
		case KEY_K:
		case KEY_L:
		case KEY_M:
		case KEY_N:
		case KEY_O:
		case KEY_P:
		case KEY_Q:
		case KEY_R:
		case KEY_S:
		case KEY_T:
		case KEY_U:
		case KEY_V:
		case KEY_W:
		case KEY_X:
		case KEY_Y:
		case KEY_Z:
			keyboard = key - 0x80;
			break;

		case KEY_ZERO:			keyboard = shift ? 0xA9 : 0xB0; break;             // 0 )
		case KEY_ONE:			keyboard = shift ? 0xA1 : 0xB1; break;             // 1 !
		case KEY_TWO:			keyboard = shift ? 0xC0 : 0xB2; break;             // 2 @
		case KEY_THREE:         keyboard = shift ? 0xA3 : 0xB3; break;             // 3 #
		case KEY_FOUR:			keyboard = shift ? 0xA4 : 0xB4; break;             // 4 $
		case KEY_FIVE:			keyboard = shift ? 0xA5 : 0xB5; break;             // 5 %
		case KEY_SIX:			keyboard = shift ? 0xDE : 0xB6; break;             // 6 ^
		case KEY_SEVEN:         keyboard = shift ? 0xA6 : 0xB7; break;             // 7 &
		case KEY_EIGHT:         keyboard = shift ? 0xAA : 0xB8; break;             // 8 *
		case KEY_NINE:			keyboard = shift ? 0xA8 : 0xB9; break;             // 9 (


		case KEY_LEFT_BRACKET:	keyboard = shift ? 0x9B : 0xDB;   break;   // [ {
		case KEY_BACKSLASH:		keyboard = shift ? 0x9C : 0xDC;   break;   // \ |
		case KEY_RIGHT_BRACKET:	keyboard = shift ? 0x9D : 0xDD;   break;   // ] }

		case KEY_APOSTROPHE:    keyboard = shift ? 0xA2 : 0xA7;   break;   // ' "
		case KEY_COMMA:			keyboard = shift ? 0xBC : 0xAC;   break;   // , <
		case KEY_PERIOD:		keyboard = shift ? 0xBE : 0xAE;   break;   // . >

		case KEY_MINUS:			keyboard = shift ? 0xDF : 0xAD;   break;	// - _
		case KEY_SLASH:			keyboard = shift ? 0xBF : 0xAF;   break;	// / ?
		case KEY_SEMICOLON :	keyboard = shift ? 0xBA : 0xBB;   break;	// ; :
		case KEY_EQUAL:			keyboard = shift ? 0xAB : 0xBD;   break;	// = +

		case KEY_BACKSPACE:		keyboard = 0x88;	break;             // BS
 		case KEY_LEFT:			keyboard = 0x88;    break;             // BS
 		case KEY_RIGHT:			keyboard = 0x95;    break;             // NAK
		case KEY_SPACE:			keyboard = 0xA0;    break;
		case KEY_ESCAPE:		keyboard = 0x9B;    break;             // ESC
		case KEY_ENTER:			keyboard = 0x8D;    break;             // CR
	}

// 	if (key != 0)
// 	{
// 		printf("KEY : %x --> %x\n", key, keyboard);
// 	}

}

BYTE tries = 0;
// 플로피 디스크 업데이트
bool Apple2Device::UpdateFloppyDisk()
{
	// until motor is off or i reaches 255+1=0
	return disk[currentDrive].motorOn && ++tries;
}


void Apple2Device::InsetFloppy()
{
	memset(&disk[0], 0, sizeof(drive));
	memset(&disk[1], 0, sizeof(drive));

	insertFloppy("rom/DOS3.3.nib", 0);
	//insertFloppy("rom/LodeRunner.nib", 0);
	
}

bool Apple2Device::GetDiskMotorState()
{
	return disk[currentDrive].motorOn;
}