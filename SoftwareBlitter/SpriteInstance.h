#ifndef __SPRITEINSTANCE_H__
#define __SPRITEINSTANCE_H__

#ifndef __SOFTSPRITE_H__
#include "SoftSprite.h"
#endif

#ifndef __VECTOR2D_H__
#include "Library/Vector2D.h"
#endif

class SpriteInstance
{
public:
	SpriteInstance(const char* spriteId);

	~SpriteInstance();

	void Render();

	void SetPosition(Vector2D position);

	void SetFlip(bool flip);

	void SetDepth(char depth);

	SoftSprite* GetSpriteData() const;

	const Vector2D& GetPosition() const;

	bool GetFlip() const;

	char GetDepth() const;

private:

	Vector2D m_position;

	bool m_flip;

	// lower = closer
	char m_depth;

	SoftSprite* m_sprite;
};

#endif
