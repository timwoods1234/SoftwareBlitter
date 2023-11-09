#ifndef _UISPRITE_H_
#define _UISPRITE_H_

#include "SoftSprite.h"
#include "UIObject.h"

class UISprite : public UIObject
{
public:
	UISprite(void);

	~UISprite(void);

	virtual const char* GetType(){return "SPRITE";};
	
	void LoadSprite(char* filename);

	virtual void onLostDevice();

	virtual void Update(float elapsedTime);

	virtual void Render();
	
private:

	Vector2D m_min;

	Vector2D m_max;

	SoftSprite* m_sprite;

};

#endif _UISPRITE_H_
