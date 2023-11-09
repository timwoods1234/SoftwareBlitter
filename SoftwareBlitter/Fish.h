#ifndef __FISH_H__
#define __FISH_H__

#ifndef __VECTOR2D_H__
#include "Library/Vector2D.h"
#endif

class SpriteInstance;

class Fish
{
public:
	Fish();

	~Fish();

	void Update(float elapsedTime);

	void Render();

private:
	static Vector2D GetRandomDirection();

	void SetInitialState();

	void SetDirection(const Vector2D& direction);

	SpriteInstance* m_sprite;

	Vector2D m_position;

	Vector2D m_direction;

	float m_speed;
};

#endif
