#include <stdlib.h>
#include "AppleFont.h"
#include "raylib.h"

AppleFont::AppleFont() 
{
}

AppleFont::~AppleFont() 
{
}

void AppleFont::Create()
{
	int w, h;
	unsigned char* img = read_bmp("font-normal.bmp", &w, &h);
	for (int n = 0; n < FONT_NUM; n++)
	{
		font[n].num = n;
		int pos = 0;
		for (int y = 0; y < FONT_Y; y++)
			for (int x = 0; x < FONT_X; x++)
				font[n].data[pos++] = img[y * w + (x + n * FONT_X)];
 	}
	free(img);

	img = read_bmp("font-reverse.bmp", &w, &h);
	for (int n = 0; n < FONT_NUM; n++)
	{
		invfont[n].num = n;
		int pos = 0;
		for (int y = 0; y < FONT_Y; y++)
			for (int x = 0; x < FONT_X; x++)
				invfont[n].data[pos++] = img[y * w + (x + n * FONT_X)];
	}

	free(img);
}

void AppleFont::RenderFont(int fontnum, int posx, int posy, bool inv)
{
	int pos = 0;
	for(int y=0; y<FONT_Y; y++)
		for (int x = 0; x < FONT_X; x++)
		{
			unsigned char c = inv ? invfont[fontnum].data[pos++] : font[fontnum].data[pos++];
			if( c == 1)
				DrawPixel(posx + x, posy + y, GREEN);
		}
}

unsigned char* AppleFont::read_bmp(const char* fname, int* _w, int* _h)
{
	unsigned char head[54];
	FILE* f = NULL;
	fopen_s(&f, fname, "rb");

	// BMP header
	fread(head, 1, 54, f);

	int w = head[18] + (((int)head[19]) << 8) + (((int)head[20]) << 16) + (((int)head[21]) << 24);
	int h = head[22] + (((int)head[23]) << 8) + (((int)head[24]) << 16) + (((int)head[25]) << 24);

	// lines are aligned on 4-byte boundary
	int lineSize = (w / 8 + (w / 8) % 4);
	int fileSize = lineSize * h;

	unsigned char* img = (unsigned char*)malloc(w * h);
	unsigned char* data = (unsigned char*)malloc(fileSize);

	// skip
	fseek(f, 54, SEEK_SET);
	// skip palette
	fseek(f, 8, SEEK_CUR);

	fread(data, 1, fileSize, f);

	// decode bits
	int i, j, k, rev_j;
	for (j = 0, rev_j = h - 1; j < h; j++, rev_j--)
	{
		for (i = 0; i < w / 8; i++) {
			int fpos = j * lineSize + i, pos = rev_j * w + i * 8;
			for (k = 0; k < 8; k++)
				img[pos + (7 - k)] = (data[fpos] >> k) & 1;
		}
	}

	free(data);
	*_w = w; *_h = h;
	return img;
}
