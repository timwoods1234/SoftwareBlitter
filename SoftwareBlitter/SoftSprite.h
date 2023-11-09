#ifndef __SOFTSPRITE_H__
#define __SOFTSPRITE_H__

struct RenderParams;
class Bitmap;

class SoftSprite
{
public:
	SoftSprite(const char* filename);

	~SoftSprite();

	int GetWidth()  const;

	int GetHeight() const;

	const Bitmap* GetRawData() const;

	void ApplyCompression(int percentage);

	void EnableTransparency(bool enable);

	bool UsesTransparency() const;

protected:

	Bitmap* m_bitmap;

	bool m_enableTransparency;
};

#endif
