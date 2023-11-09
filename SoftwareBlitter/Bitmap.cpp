
#include <vector>

#include "Library/Error.h"

#include "Bitmap.h"

/*****************************************************************************************/
Bitmap::Bitmap()
{
	m_lastError = "Ok";
}

/*****************************************************************************************/
Bitmap::~Bitmap()
{
	if (m_palette != NULL)
	{
		free(m_palette);
	}

	if (m_data != NULL)
	{
		free(m_data);
	}
}

/*****************************************************************************************/
bool Bitmap::ReadFile(const char* filename)
{
	FILE* f;
	fopen_s(&f, filename, "rb");

	if (!ReadHeader(f) || m_header.Magic != 0x4D42)
	{
		m_lastError = "Invalid file";
		fclose(f);
		return false;
	}

	if ((m_header.BitsPerPixel != 32 && m_header.BitsPerPixel != 24 && m_header.BitsPerPixel != 8) || m_header.CompressionType != 0 || m_header.HeaderSize != 40)
	{
		m_lastError = "Format not supported";
		fclose(f);
		return false;
	}

	if (m_header.BitsPerPixel == 8)
	{
		m_palette = (unsigned char*)malloc(BPP8_PALETTE_SIZE * sizeof(unsigned char));
		if (m_palette == NULL)
		{
			m_lastError = "Out of memory";
			fclose(f);
			return false;
		}

		if (fread(m_palette, sizeof(unsigned char), BPP8_PALETTE_SIZE, f) != BPP8_PALETTE_SIZE)
		{
			m_lastError = "File inavlid";
			fclose(f);
			free(m_palette);
			return false;
		}
	}
	else
	{
		// image doesn't use palette
		m_palette = NULL;
	}

	m_data = (unsigned char*)malloc(m_header.ImageDataSize);

	if (m_data == NULL)
	{
		m_lastError = "Out of memory";
		fclose(f);
		free(m_palette);
		return false;
	}

	if (fread(m_data, sizeof(unsigned char), m_header.ImageDataSize, f) != m_header.ImageDataSize)
	{
		m_lastError = "Invalid file";
		fclose(f);
		free(m_data);
		free(m_palette);

		return false;
	}

	fclose(f);

	return true;
}

/*****************************************************************************************/
void Bitmap::GetPixelRGB(unsigned int x, unsigned int y, unsigned char *r, unsigned char *g, unsigned char *b) const
{
	// stride
	unsigned char	bytesPerPixel = m_header.BitsPerPixel >> 3;
	unsigned int	bytesPerRow = m_header.ImageDataSize / m_header.Height;

	// image is upside down
	unsigned char* pixel = m_data + ((m_header.Height - y - 1) * bytesPerRow + x * bytesPerPixel);

	if (m_header.BitsPerPixel == 8)
	{
		pixel = m_palette + *pixel * 4;
	}

	// store in BGR order
	if (r)	*r = *(pixel + 2);
	if (g)	*g = *(pixel + 1);
	if (b)	*b = *(pixel + 0);
}

/*****************************************************************************************/
void Bitmap::SetPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char bytesPerPixel = m_header.BitsPerPixel >> 3;
	unsigned int bytesPerRow = m_header.ImageDataSize / m_header.Height;

	unsigned char *pixel = m_data + ((m_header.Height - y - 1) * bytesPerRow + x * bytesPerPixel);

	*(pixel + 2) = r;
	*(pixel + 1) = g;
	*(pixel + 0) = b;
}

/*****************************************************************************************/
void Bitmap::GetPixelIndex(unsigned int x, unsigned int y, unsigned char* value) const
{
	unsigned int bytesPerRow = m_header.ImageDataSize / m_header.Height;

	unsigned char *pixel = m_data + ((m_header.Height - y - 1) * bytesPerRow + x);

	if (value) *value = *pixel;
}

/*****************************************************************************************/
void Bitmap::SetPixelIndex(unsigned int x, unsigned int y, unsigned char value)
{
	unsigned int bytesPerRow = m_header.ImageDataSize / m_header.Height;

	unsigned char *pixel = m_data + ((m_header.Height - y - 1) * bytesPerRow + x);

	*pixel = value;
}

/*****************************************************************************************/
void Bitmap::GetPaletteColor(unsigned char index, unsigned char* r, unsigned char* g, unsigned char* b) const
{
	if (m_header.BitsPerPixel != 8)
	{
		m_lastError = "Type Mismatch";
	}
	else
	{
		if (r) *r = *(m_palette + index * 4 + 2);
		if (g) *g = *(m_palette + index * 4 + 1);
		if (b) *b = *(m_palette + index * 4 + 0);
	}
}

/*****************************************************************************************/
void Bitmap::SetPaletteColor(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
	if (m_header.BitsPerPixel != 8)
	{
		m_lastError = "Type Mismatch";
	}
	else
	{
		// write directly to palette
		*(m_palette + index * 4 + 2) = r;
		*(m_palette + index * 4 + 1) = g;
		*(m_palette + index * 4 + 0) = b;
	}
}

/*****************************************************************************************/
void Bitmap::CompressPalette(int tolerance)
{
	std::vector<unsigned char> newPalette;
	newPalette.reserve(m_header.Width);

	unsigned char r, g, b;

	// add initial color to palette (top left pixel)
	GetPixelRGB(0, 0, &r, &g, &b);
	newPalette.push_back(b);
	newPalette.push_back(g);
	newPalette.push_back(r);
	newPalette.push_back(0);	// round up to 4 bits

	for (unsigned int y = 0; y < GetHeight(); y++)
	{
		for (unsigned int x = 0; x < GetWidth(); x++)
		{
			GetPixelRGB(x, y, &r, &g, &b);

			// add color to new Palette (if needed)
			bool found = false;
			unsigned int i = 0;

			do
			{
				// is color in palette[i]?
				if ((newPalette[i + 2] - tolerance <= r && r <= newPalette[i + 2] + tolerance)
					&& (newPalette[i + 1] - tolerance <= g && g <= newPalette[i + 1] + tolerance)
					&& (newPalette[i + 0] - tolerance <= b && b <= newPalette[i + 0] + tolerance))
				{
					// yes, set this pixel to point to it
					SetPixelIndex(x, y, i / 4);
					found = true;

					break;
				}

				i += 4;

			} while (i < newPalette.size());

			if (!found)
			{
				// no, add color to palette
				newPalette.push_back(b);
				newPalette.push_back(g);
				newPalette.push_back(r);
				newPalette.push_back(0);	// round up to 4 bits

											// and set pixel pointer
				SetPixelIndex(x, y, i / 4);
			}
		}
	}

	if (m_palette != NULL)
	{
		free(m_palette);
	}

	m_palette = (unsigned char*)malloc(newPalette.size() * sizeof(unsigned char));

	if (m_palette == NULL)
	{
		m_lastError = "Out of memory";
		return;
	}

	DebugPrint("Palette Compression Reduced %d colours to %d", BPP8_PALETTE_SIZE, newPalette.size());

	for (unsigned int i = 0; i < newPalette.size(); i++)
	{
		m_palette[i] = newPalette[i];
	}

	newPalette.clear();

	m_header.BitsPerPixel = 8;	// treat this bitmap as an 8BPP image from now on (i.e. use palette colors)
}

/*****************************************************************************************/
bool Bitmap::ReadHeader(FILE* f)
{
	if (f == NULL)
	{
		return false;
	}

	if (!ReadShort(&(m_header.Magic), f))			return false;
	if (!ReadInt(&(m_header.FileSize), f))			return false;
	if (!ReadShort(&(m_header.Reserved1), f))		return false;
	if (!ReadShort(&(m_header.Reserved2), f))		return false;
	if (!ReadInt(&(m_header.DataOffset), f))		return false;
	if (!ReadInt(&(m_header.HeaderSize), f))		return false;
	if (!ReadInt(&(m_header.Width), f))				return false;
	if (!ReadInt(&(m_header.Height), f))			return false;
	if (!ReadShort(&(m_header.Planes), f))			return false;
	if (!ReadShort(&(m_header.BitsPerPixel), f))	return false;
	if (!ReadInt(&(m_header.CompressionType), f))	return false;
	if (!ReadInt(&(m_header.ImageDataSize), f))		return false;
	if (!ReadInt(&(m_header.HPixelsPerMeter), f))	return false;
	if (!ReadInt(&(m_header.VPixelsPerMeter), f))	return false;
	if (!ReadInt(&(m_header.ColorsUsed), f))		return false;
	if (!ReadInt(&(m_header.ColorsRequired), f))	return false;

	return true;
}

/*****************************************************************************************/
bool Bitmap::WriteHeader(FILE* f)
{
	if (f == NULL)
	{
		return false;
	}

	if (!WriteShort(m_header.Magic, f))			return false;
	if (!WriteInt(m_header.FileSize, f))		return false;
	if (!WriteShort(m_header.Reserved1, f))		return false;
	if (!WriteShort(m_header.Reserved2, f))		return false;
	if (!WriteInt(m_header.DataOffset, f))		return false;
	if (!WriteInt(m_header.HeaderSize, f))		return false;
	if (!WriteInt(m_header.Width, f))			return false;
	if (!WriteInt(m_header.Height, f))			return false;
	if (!WriteShort(m_header.Planes, f))		return false;
	if (!WriteShort(m_header.BitsPerPixel, f))	return false;
	if (!WriteInt(m_header.CompressionType, f))	return false;
	if (!WriteInt(m_header.ImageDataSize, f))	return false;
	if (!WriteInt(m_header.HPixelsPerMeter, f))	return false;
	if (!WriteInt(m_header.VPixelsPerMeter, f))	return false;
	if (!WriteInt(m_header.ColorsUsed, f))		return false;
	if (!WriteInt(m_header.ColorsRequired, f))	return false;

	return true;
}

/*****************************************************************************************/
bool Bitmap::ReadInt(unsigned int *x, FILE* f)
{
	unsigned char little[4];

	if (x == NULL || f == NULL)
	{
		return false;
	}

	if (fread(little, 4, 1, f) != 1)
	{
		return false;
	}

	*x = (little[3] << 24 | little[2] << 16 | little[1] << 8 | little[0]);

	return true;
}

/*****************************************************************************************/
bool Bitmap::ReadShort(unsigned short *x, FILE* f)
{
	unsigned char little[2];

	if (x == NULL || f == NULL)
	{
		return false;
	}

	if (fread(little, 2, 1, f) != 1)
	{
		return false;
	}

	*x = (little[1] << 8 | little[0]);

	return true;
}

/*****************************************************************************************/
int Bitmap::WriteInt(unsigned int x, FILE* f)
{
	unsigned char little[4];

	little[3] = (unsigned char)((x & 0xff000000) >> 24);
	little[2] = (unsigned char)((x & 0x00ff0000) >> 16);
	little[1] = (unsigned char)((x & 0x0000ff00) >> 8);
	little[0] = (unsigned char)((x & 0x000000ff) >> 0);

	return (f && fwrite(little, 4, 1, f) == 1);
}

/*****************************************************************************************/
int Bitmap::WriteShort(unsigned short x, FILE* f)
{
	unsigned char little[2];

	little[1] = (unsigned char)((x & 0xff00) >> 8);
	little[0] = (unsigned char)((x & 0x00ff) >> 0);

	return (f && fwrite(little, 2, 1, f) == 1);
}

/*****************************************************************************************/
char* Bitmap::GetLastError() const
{
	return m_lastError;
}

/*****************************************************************************************/
unsigned int Bitmap::GetWidth()  const
{
	return m_header.Width;
}

/*****************************************************************************************/
unsigned int Bitmap::GetHeight() const
{
	return m_header.Height;
}

/*****************************************************************************************/
unsigned short Bitmap::GetDepth()  const
{
	return m_header.BitsPerPixel;
}