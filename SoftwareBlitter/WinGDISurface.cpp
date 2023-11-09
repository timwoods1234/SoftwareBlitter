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
void WinGDISurface::AddSprite(SpriteInstance* sprite)
{
	const Bitmap* bitmap = sprite->GetSpriteData()->GetRawData();

	int spriteWidth = bitmap->GetWidth();
	int spriteHeight = bitmap->GetHeight();
	bool enableTransparency = sprite->GetSpriteData()->UsesTransparency();

	int spriteActualPositionY = m_bufferHeight - spriteHeight - (int)sprite->GetPosition().y;
	int destinationIndex = spriteActualPositionY * m_bufferWidth + (int)sprite->GetPosition().x;

	int sourceX = sprite->GetFlip() ? (spriteWidth - 1) : 0;
	int sourceDirection = sprite->GetFlip() ? -1 : 1;

	unsigned char r, g, b;
	bool validDepth = true;
	unsigned int transparencyColour = -1;

	if (enableTransparency)
	{
		bitmap->GetPixelRGB(0, 0, &r, &g, &b);
		transparencyColour = RGB32(r, g, b);
	}

	for (int spriteY = spriteHeight - 1; spriteY >= 0; spriteY--)
	{
		for (int spriteX = 0; spriteX < spriteWidth; spriteX++)
		{
			validDepth = true;

			if (destinationIndex < m_totalPixelCount && destinationIndex >= 0)
			{
				if (m_blendMode == ALWAYS_FAIL_DEPTH && m_depthBuffer[destinationIndex] != 127)
				{
					validDepth = false;
				}
				else if (m_blendMode == COMPARE_DEPTH)
				{
					if (m_depthBuffer[destinationIndex] < sprite->GetDepth() && m_depthBuffer[destinationIndex] != -1)
					{
						validDepth = false;
					}
				}

				if (validDepth)
				{
					bitmap->GetPixelRGB(sourceX, spriteY, &r, &g, &b);
					unsigned long colour = RGB32(r, g, b);

					if ((enableTransparency && colour != transparencyColour) || !enableTransparency)
					{
						m_depthBuffer[destinationIndex] = sprite->GetDepth();
						m_pixelData[destinationIndex] = colour;
					}
				}
			}

			sourceX += sourceDirection;
			destinationIndex++;
		}

		sourceX = sprite->GetFlip() ? (spriteWidth - 1) : 0;
		destinationIndex += (m_bufferWidth - spriteWidth);
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
