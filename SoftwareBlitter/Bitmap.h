#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifndef __BITMAPHEADER_H__
#include "BitmapHeader.h"
#endif

#include <stdio.h>

// Size of the palette data for 8 BPP bitmaps
#define BPP8_PALETTE_SIZE	( 256 * 4 )

class Bitmap
{
public:
	Bitmap();

	~Bitmap();

	bool ReadFile(const char* filename);

	void CompressPalette(int tolerance);

	unsigned int GetWidth()  const;

	unsigned int GetHeight() const;

	unsigned short GetDepth()  const;
	
	void GetPixelRGB(unsigned int x, unsigned int y, unsigned char *r, unsigned char *g, unsigned char *b) const;

	void SetPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);

	void GetPixelIndex(unsigned int x, unsigned int y, unsigned char *value) const;

	void SetPixelIndex(unsigned int x, unsigned int y, unsigned char value);

	void GetPaletteColor(unsigned char index, unsigned char *r, unsigned char *g, unsigned char *b) const;

	void SetPaletteColor(unsigned char index, unsigned char r, unsigned char g, unsigned char b);

	char* GetLastError() const;

private:

	bool ReadHeader	(FILE* f);

	bool WriteHeader(FILE* f);

	bool ReadInt (unsigned int *x, FILE* f);

	bool ReadShort(unsigned short *x, FILE* f);

	int  WriteInt (unsigned int x, FILE* f);

	int  WriteShort (unsigned short x, FILE* f);

	BMP_Header m_header;

	unsigned char* m_palette;

	unsigned char* m_data;

	mutable char* m_lastError;
};

#endif
