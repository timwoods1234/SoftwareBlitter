#include "UISprite.h"

/*****************************************************************************************/
UISprite::UISprite(void)
{
	m_sprite = 0;
}

/*****************************************************************************************/
UISprite::~UISprite(void)
{
	delete m_sprite;
}

/*****************************************************************************************/
void UISprite::LoadSprite(char* filename)
{
	m_sprite = new SoftSprite();
	m_sprite->LoadSprite(filename);
	m_sprite->SetDefaultColorKey();
}

/*****************************************************************************************/
void UISprite::Render()
{
	if(m_render)
	{
		m_sprite->Render(m_position.x,m_position.y);
	}
}

/*****************************************************************************************/
void UISprite::Update(float elapsedTime)
{
	
}	

/*****************************************************************************************/
void UISprite::onLostDevice()
{

}
