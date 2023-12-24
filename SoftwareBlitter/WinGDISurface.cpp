#include "WinGDISurface.h"
#include "Bitmap.h"
#include "ColourFormatHelper.h"
#include "SpriteInstance.h"

/*****************************************************************************************/
WinGDISurface::WinGDISurface(HWND window)
{
	RECT rcClient;

	GetClientRect(window, &rcClient);

	m_bufferWidth = rcClient.right;
	m_bufferHeight = rcClient.bottom;

	m_bufferHandle = GetDC(window);
	m_doubleBufferHandle = CreateCompatibleDC(m_bufferHandle);

	m_buffer = CreateCompatibleBitmap(m_bufferHandle, m_bufferWidth, m_bufferHeight);
	SelectObject(m_doubleBufferHandle, m_buffer);

	BITMAP bm;
	GetObject(m_buffer, sizeof(bm), &bm);

	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bm.bmWidth;
	bmpInfo.bmiHeader.biHeight = bm.bmHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;

	m_doubleBuffer = CreateDIBSection(m_doubleBufferHandle,
		(BITMAPINFO *)&bmpInfo,
		DIB_RGB_COLORS,
		(void **)&m_pixelData,
		NULL, 0);

	m_totalPixelCount = bm.bmWidth * bm.bmHeight;

	m_depthBuffer = (char*)malloc(m_totalPixelCount * sizeof(char));

	SelectObject(m_doubleBufferHandle, m_doubleBuffer);
	m_clearColour = 66666;
}

/*****************************************************************************************/
WinGDISurface::~WinGDISurface()
{
	DeleteDC(m_bufferHandle);
	DeleteDC(m_doubleBufferHandle);
	DeleteObject(m_buffer);
	DeleteObject(m_doubleBuffer);

	free(m_depthBuffer);
}

/*****************************************************************************************/
void WinGDISurface::BlitToWindow()
{
	BitBlt(m_bufferHandle, 0, 0, m_bufferWidth, m_bufferHeight, m_doubleBufferHandle, 0, 0, SRCCOPY);
}

/*****************************************************************************************/
void WinGDISurface::Clear()
{
	for (int i = 0; i < m_totalPixelCount; i++)
	{
		m_pixelData[i] = m_clearColour;
		m_depthBuffer[i] = 127;
	}
}

/*****************************************************************************************/
void WinGDISurface::AddSprite(SpriteInstance* spriteInstance)
{
	unsigned int posX = (unsigned int)spriteInstance->GetPosition().x;
	unsigned int posY = (unsigned int)spriteInstance->GetPosition().y;
	char depth = spriteInstance->GetDepth();
	int direction = spriteInstance->GetFlip() ? -1 : 1;
	int srcIncrement = spriteInstance->GetFlip() ? 2 : 0;

	SoftSprite* spriteData = spriteInstance->GetSpriteData();
	const unsigned int* srcPixel = spriteData->GetRawPtr();
	const char* mask = spriteData->GetMaskPtr();
	unsigned int spriteWidth = spriteData->GetWidth();
	unsigned int spriteHeight = spriteData->GetHeight();

	char* depthBuffer = m_depthBuffer + (posY * m_bufferWidth + posX);
	unsigned int* dstPixel = m_pixelData + (posY * m_bufferWidth + posX);

	unsigned int runLength = m_bufferWidth - posX;
	unsigned int runHeight = m_bufferHeight - (unsigned int)spriteInstance->GetPosition().y;

	if (runLength > spriteWidth)
	{
		runLength = spriteWidth;
	}

	if (runHeight > spriteHeight)
	{
		runHeight = spriteHeight;
	}

	if (spriteInstance->GetFlip())
	{
		srcPixel = srcPixel + spriteWidth;
		mask = mask + spriteWidth;
	}

	bool writePixel = true;

	for (unsigned int width = 0; width < runLength; width++)
	{
		for (unsigned int height = 0; height < runHeight; height++)
		{
			writePixel = true;

			writePixel ^= (m_blendMode == ALWAYS_FAIL_DEPTH && *depthBuffer != 127);
			writePixel ^= (m_blendMode == COMPARE_DEPTH && depth >= *depthBuffer);

			/*if (m_blendMode == ALWAYS_FAIL_DEPTH && *depthBuffer < 127)
			{
				// skip written pixel
				writePixel = false;
			}
			else if (m_blendMode == COMPARE_DEPTH && depth >= *depthBuffer)
			{
				// skip writing deeper object, there is something in front of it
				writePixel = false;
			}*/

			if (writePixel)
			{
				*dstPixel = (*dstPixel & *mask) | *srcPixel;
				*depthBuffer = (*depthBuffer & *mask) | depth;
			}

			dstPixel++;
			srcPixel += direction;
			mask += direction;
			depthBuffer++;
		}

		dstPixel += (m_bufferWidth - spriteWidth);
		depthBuffer += (m_bufferWidth - spriteWidth);
		srcPixel += srcIncrement * spriteWidth;
		mask += srcIncrement * spriteWidth;
	}
}

/*****************************************************************************************/
BlendMode WinGDISurface::GetBlendMode() const
{
	return m_blendMode;
}

/*****************************************************************************************/
void WinGDISurface::SetBlendMode(BlendMode mode)
{
	m_blendMode = mode;
}
