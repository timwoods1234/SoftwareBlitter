#ifndef __WINGDISURFACE_H__
#define __WINGDISURFACE_H__

#include <windows.h>

#ifndef __BLENDMODE_H__
#include "BlendMode.h"
#endif

class SpriteInstance;

class WinGDISurface
{
public:
	WinGDISurface(HWND window);

	~WinGDISurface();

	void BlitToWindow();

	void Clear();

	void AddSprite(SpriteInstance* sprite);

	BlendMode GetBlendMode() const;

	void SetBlendMode(BlendMode mode);

private:
	HBITMAP m_buffer;
	HDC		m_bufferHandle;

	HBITMAP m_doubleBuffer;
	HDC		m_doubleBufferHandle;

	unsigned int* m_pixelData;
	char* m_depthBuffer;

	unsigned int m_clearColour;

	int m_bufferWidth;

	int m_bufferHeight;

	int m_totalPixelCount;

	BlendMode m_blendMode;
};

#endif
