#ifndef APPLE2_FONT_H
#define APPLE2_FONT_H

#include "Predef.h"

#define FONT_X		7
#define FONT_Y		8
#define FONT_NUM	128

#define FONT_NORMAL			0
#define FONT_INVERSE		1
#define FONT_FLASH			2

class AppleFont
{
private:
	unsigned char font[FONT_NUM][FONT_X*FONT_Y];
	unsigned char invfont[FONT_NUM][FONT_X*FONT_Y];
	unsigned char* read_bmp(const char* fname, int* _w, int* _h);

public:
	AppleFont();
	~AppleFont();

	void Create();
	void RenderFont(int fontnum, int x, int y, bool inv);

};



#endif