
#include "SoftSprite.h"
#include "Bitmap.h"
#include "ColourFormatHelper.h"
#include "SoftwareRenderer.h"

/*****************************************************************************************/
SoftSprite::SoftSprite(const char* filename)
{
	m_enableTransparency = false;

	m_bitmap = new Bitmap();
	m_bitmap->ReadFile(filename);

	int spriteWidth = m_bitmap->GetWidth();
	int spriteHeight = m_bitmap->GetHeight();

	m_cachedPixels = new unsigned int[spriteWidth * spriteHeight];
	m_mask = new char[spriteWidth * spriteHeight];

	BuildCachedBuffers();
}

/*****************************************************************************************/
SoftSprite::~SoftSprite()
{
	delete m_cachedPixels;
	delete m_bitmap;
	delete m_mask;
}

/*****************************************************************************************/
const char* SoftSprite::GetMaskPtr() const
{
	return m_mask;
}

/*****************************************************************************************/
unsigned int SoftSprite::GetColourKey() const
{
	unsigned char r, g, b;

	m_bitmap->GetPixelRGB(0, 0, &r, &g, &b);

	return RGB32(r, g, b);
}

/*****************************************************************************************/
const unsigned int* SoftSprite::GetRawPtr() const
{
	return m_cachedPixels;
}

/*****************************************************************************************/
unsigned int SoftSprite::GetWidth()  const
{
	return m_bitmap->GetWidth();
}

/*****************************************************************************************/
unsigned int SoftSprite::GetHeight() const
{
	return m_bitmap->GetHeight();
}

/*****************************************************************************************/
void SoftSprite::ApplyCompression(int percentage)
{
	m_bitmap->CompressPalette(percentage);
}

/*****************************************************************************************/
void SoftSprite::EnableTransparency(bool enable)
{
	if (m_enableTransparency != enable)
	{
		m_enableTransparency = enable;
		BuildCachedBuffers();
	}
}

/*****************************************************************************************/
bool SoftSprite::UsesTransparency() const
{
	return m_enableTransparency;
}

/*****************************************************************************************/
void SoftSprite::BuildCachedBuffers()
{
	int spriteWidth = m_bitmap->GetWidth();
	int spriteHeight = m_bitmap->GetHeight();
	unsigned int colourKey = GetColourKey();

	unsigned char r, g, b;

	for (int width = 0; width < spriteWidth; width++)
	{
		for (int height = 0; height < spriteHeight; height++)
		{
			m_bitmap->GetPixelRGB(width, (spriteHeight - 1 - height), &r, &g, &b);
			unsigned long colour = RGB32(r, g, b);

			unsigned int index = spriteWidth * height + width;

			if (m_enableTransparency && colour == colourKey)
			{
				m_mask[index] = 127;
				m_cachedPixels[index] = 0;
			}
			else
			{
				m_mask[index] = 0;
				m_cachedPixels[index] = colour;
			}
		}
	}
}
