#ifndef __SOFTSPRITE_H__
#define __SOFTSPRITE_H__

struct RenderParams;
class Bitmap;

class SoftSprite
{
public:
	SoftSprite(const char* filename);

	~SoftSprite();

	unsigned int GetWidth()  const;

	unsigned int GetHeight() const;

	// return the top-left pizel colour
	unsigned int GetColourKey() const;

	const unsigned int* GetRawPtr() const;

	const char* GetMaskPtr() const;

	void ApplyCompression(int percentage);

	void EnableTransparency(bool enable);

	bool UsesTransparency() const;

protected:

	void BuildCachedBuffers();

	Bitmap* m_bitmap;

	unsigned int* m_cachedPixels;

	char* m_mask;

	bool m_enableTransparency;
};

#endif
