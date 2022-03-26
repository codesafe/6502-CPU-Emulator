

#include "Predef.h"
#include "AppleCPU.h"
#include "AppleMem.h"
#include "Apple2device.h"
#include "AppleFont.h"
#include "raylib.h"


drive disk[2] = { 0 };

Apple2Device::Apple2Device()
{
}

Apple2Device::~Apple2Device()
{
	mem.Destroy();
}

void Apple2Device::InitDevice()
{
	mem.Create();
	font.Create();

	currentDrive = 0;
	textMode = true;
	mixedMode = false;
	videoPage = 1;
	hires_Mode = false;
	keyboard = 0;
	videoAddress = videoPage * 0x0400;
}

bool Apple2Device::UploadRom()
{
	bool ret = false;
	BYTE* rom = new BYTE[ROMSIZE];
	FILE* fp = fopen("rom/appleII+.rom", "rb"); // load the Apple II+ ROM
	if (fp) 
	{
		fread(rom, ROMSIZE, 1, fp);
		mem.UpLoadProgram(ROMSTART, rom, ROMSIZE);
		fclose(fp);
		ret = true;
	}
	delete[] rom;

	cpu.Reset(mem);
	return ret;
}

// 소프트 스위칭
void Apple2Device::SoftSwitch()
{
	if (mem.softswitch.switched)
	{
		WORD address = mem.softswitch.address;
		BYTE value = mem.softswitch.value;
		bool WRT = mem.softswitch.readorwrite;

		ExecSoftSwitch(address, value, WRT);
	}

}

// Language Card writable
bool    LCWR = true;                                                           
// Language Card readable
bool LCRD = false;                                                          
bool LCBK2 = true; // Language Card bank 2 enabled
bool LCWFF = false; // Language Card pre-write flip flop

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

BYTE Apple2Device::ExecSoftSwitch(WORD address, BYTE value, bool WRT)
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
			playSound(); 
			break; // apple invader uses $C033 to output sound !

		// Graphics
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

		case 0xC061: return(PB0);                                                   // Push Button 0
		case 0xC062: return(PB1);                                                   // Push Button 1
		case 0xC063: return(PB2);                                                   // Push Button 2
// 		case 0xC064: return(readPaddle(0));                                         // Paddle 0
// 		case 0xC065: return(readPaddle(1));                                         // Paddle 1
// 		case 0xC066: return(readPaddle(0));                                         // Paddle 2 -- not implemented
// 		case 0xC067: return(readPaddle(1));                                         // Paddle 3 -- not implemented

//		case 0xC070: resetPaddles(); break;                                         // paddle timer RST

//		case 0xC0E0 ... 0xC0E7: stepMotor(address); break;                          // MOVE DRIVE HEAD

// 		case 0xCFFF:
// 		case 0xC0E8: disk[curDrv].motorOn = false; break;                           // MOTOROFF
// 		case 0xC0E9: disk[curDrv].motorOn = true;  break;                           // MOTORON
// 
// 		case 0xC0EA: setDrv(0); break;                                              // DRIVE0EN
// 		case 0xC0EB: setDrv(1); break;                                              // DRIVE1EN

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

		case 0xC0ED: dLatch = value; break;                                         // Load Data Latch

		// latch for READ
		case 0xC0EE:
			disk[currentDrive].writeMode = false;
			return(disk[currentDrive].readOnly ? 0x80 : 0);                                 // check protection
		// latch for WRITE
		case 0xC0EF: 
			disk[currentDrive].writeMode = true; 
			break;

		// LANGUAGE CARD :
		// LC2RD
		case 0xC080:                                                                
		case 0xC084: 
			LCBK2 = 1; LCRD = 1; LCWR = 0;    LCWFF = 0;    
			break;
		// LC2WR		
		case 0xC081:
		case 0xC085: 
			LCBK2 = 1; LCRD = 0; LCWR |= LCWFF; LCWFF = !WRT; 
			break;
		// ROMONLY2		
		case 0xC082:
		case 0xC086: 
			LCBK2 = 1; LCRD = 0; LCWR = 0;    LCWFF = 0;    
			break;

		case 0xC083:
		case 0xC087: LCBK2 = 1; LCRD = 1; LCWR |= LCWFF; LCWFF = !WRT; break;         // LC2RW
		case 0xC088:
		case 0xC08C: LCBK2 = 0; LCRD = 1; LCWR = 0;    LCWFF = 0;    break;         // LC1RD
		case 0xC089:
		case 0xC08D: LCBK2 = 0; LCRD = 0; LCWR |= LCWFF; LCWFF = !WRT; break;         // LC1WR
		case 0xC08A:
		case 0xC08E: LCBK2 = 0; LCRD = 0; LCWR = 0;    LCWFF = 0;    break;         // ROMONLY1
		case 0xC08B:
		case 0xC08F: LCBK2 = 0; LCRD = 1; LCWR |= LCWFF; LCWFF = !WRT; break;         // LC1RW
	}

	return 0;//(ticks % 256);                                                            // catch all
}


void Apple2Device::playSound()
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

void Apple2Device::Render(int frame)
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
				BYTE glyph = mem.GetByte(videoAddress + offsetGR[line] + col);

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



