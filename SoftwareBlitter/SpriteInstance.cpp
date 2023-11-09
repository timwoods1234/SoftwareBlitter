#include "SpriteInstance.h"
#include "SoftwareRenderer.h"

/*****************************************************************************************/
SpriteInstance::SpriteInstance(const char* spriteId)
{
	m_sprite = new SoftSprite(spriteId);

	m_flip = false;

	m_depth = 0;
}

/*****************************************************************************************/
SpriteInstance::~SpriteInstance()
{
	delete m_sprite;
}

/*****************************************************************************************/
void SpriteInstance::Render()
{
	SoftwareRenderer::GetCurrentInstance()->Render(this);
}

/*****************************************************************************************/
void SpriteInstance::SetPosition(Vector2D position)
{
	m_position = position;
}

/*****************************************************************************************/
void SpriteInstance::SetFlip(bool flip)
{
	m_flip = flip;
}

/*****************************************************************************************/
void SpriteInstance::SetDepth(char depth)
{
	m_depth = depth;
}

/*****************************************************************************************/
SoftSprite* SpriteInstance::GetSpriteData() const
{
	return m_sprite;
}

/*****************************************************************************************/
const Vector2D& SpriteInstance::GetPosition() const
{
	return m_position;
}

/*****************************************************************************************/
bool SpriteInstance::GetFlip() const
{
	return m_flip;
}

/*****************************************************************************************/
char SpriteInstance::GetDepth() const
{
	return m_depth;
}