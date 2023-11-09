#ifndef __SOFTWARERENDER_H__
#define __SOFTWARERENDER_H__

#include <windows.h>

#ifndef __BLENDMODE_H__
#include "BlendMode.h"
#endif

class SpriteInstance;
class WinGDISurface;

class SoftwareRenderer
{
public:
	SoftwareRenderer(HWND window);

	~SoftwareRenderer();

	void Reset();

	void Present();

	void Render(SpriteInstance* spriteInstance);

	BlendMode GetBlendMode() const;

	void SetBlendMode(BlendMode mode);

	static SoftwareRenderer* GetCurrentInstance();

	static void Create(HWND window);

	static void Destroy();

private:
	static SoftwareRenderer* s_currentInstance;

	WinGDISurface* m_surface;
};

#endif
