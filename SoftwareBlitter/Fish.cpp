
#include <stdlib.h>

#include "Fish.h"
#include "SoftSprite.h"
#include "SpriteInstance.h"
#include "Settings.h"
#include "Library/Error.h"

/*****************************************************************************************/
Fish::Fish()
{
	m_sprite = new SpriteInstance("Assets/Fish.bmp");

	m_sprite->SetDepth(-2);
	m_sprite->GetSpriteData()->EnableTransparency(true);

	SetInitialState();
}

/*****************************************************************************************/
Fish::~Fish()
{
	delete m_sprite;
}

/*****************************************************************************************/
void Fish::Render()
{
	m_sprite->SetPosition(m_position);
	m_sprite->Render();
}

/*****************************************************************************************/
void Fish::Update(float elapsedTime)
{
	m_position += m_direction * elapsedTime * m_speed;

	Vector2D min(0.f, 0.f);
	Vector2D max((float)(Settings::ScreenWidth - m_sprite->GetSpriteData()->GetWidth()), (float)(Settings::ScreenHeight - m_sprite->GetSpriteData()->GetHeight()));

	if (m_position.x < min.x)
	{
		m_direction.x = m_direction.x * -1.f;
		m_position.x = min.x;
	}
	else if (m_position.x >= max.x)
	{
		m_direction.x = m_direction.x * -1.f;
		m_position.x = max.x;
	}

	if (m_position.y < min.y)
	{
		m_direction.y = m_direction.y * -1.f;
		m_position.y = min.y;
	}
	else if (m_position.y >= max.y)
	{
		m_direction.y = m_direction.y * -1.f;
		m_position.y = max.y;
	}

	m_sprite->SetFlip(m_direction.x > 0);
}

/*****************************************************************************************/
void Fish::SetInitialState()
{
	m_position.Set(20.f, (float)(Settings::ScreenHeight * 0.5f));
	m_speed = 80.f;

	Vector2D initialDirection = GetRandomDirection();
	m_direction = initialDirection;
}

/*****************************************************************************************/
Vector2D Fish::GetRandomDirection()
{
	Vector2D result;

	float randomX = rand() * 0.5f + 0.5f;
	float randomY = rand() * 0.5f + 0.5f;

	result.Set(randomX, randomY);
	result.Normalize();

	if (result.x < 0.2f)
	{
		result.x = 0.2f;
	}

	if (result.x < 0.2f)
	{
		result.y = 0.2f;
	}

	return result;
}
