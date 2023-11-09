
#include "SoftSprite.h"
#include "Bitmap.h"
#include "ColourFormatHelper.h"
#include "SoftwareRenderer.h"

/*****************************************************************************************/
SoftSprite::SoftSprite(const char* filename)
{
	m_bitmap = new Bitmap();
	m_bitmap->ReadFile(filename);
}

/*****************************************************************************************/
SoftSprite::~SoftSprite()
{
	delete m_bitmap;
}

/*****************************************************************************************/
const Bitmap* SoftSprite::GetRawData() const
{
	return m_bitmap;
}

/*****************************************************************************************/
int SoftSprite::GetWidth()  const
{
	return m_bitmap->GetWidth();
}

/*****************************************************************************************/
int SoftSprite::GetHeight() const
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
	m_enableTransparency = enable;
}

/*****************************************************************************************/
bool SoftSprite::UsesTransparency() const
{
	return m_enableTransparency;
}