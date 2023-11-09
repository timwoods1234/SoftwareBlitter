#ifndef __BITMAPHEADER_H__
#define __BITMAPHEADER_H__

struct BMP_Header
{
	unsigned short		Magic;				// Bitmap Identifier
	unsigned int		FileSize;			// Size of the file (bytes)
	unsigned short		Reserved1;			// Reserved by OS
	unsigned short		Reserved2;
	unsigned int		DataOffset;			// Offset of image data
	unsigned int		HeaderSize;			// Header size (bytes)
	unsigned int		Width;				// Image width
	unsigned int		Height;				// Image height
	unsigned short		Planes;
	unsigned short		BitsPerPixel;		// Bit depth
	unsigned int		CompressionType;	// Currently unused
	unsigned int		ImageDataSize;		// Size of image data
	unsigned int		HPixelsPerMeter;	// Horizontal resolution
	unsigned int		VPixelsPerMeter;	// Vertical resolution
	unsigned int		ColorsUsed;			// Number of color indexes in the color table that are actually used by the bitmap
	unsigned int		ColorsRequired;		// Number of color indexes that are required for displaying the bitmap
};

#endif
