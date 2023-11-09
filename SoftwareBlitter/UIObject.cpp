#include "UIObject.h"
/*****************************************************************************************/
UIObject::UIObject(void)
{
}

/*****************************************************************************************/
UIObject::~UIObject(void)
{
}

/*****************************************************************************************/
void UIObject::SetID(const char* ID)
{
	m_ID = ID;
}

/*****************************************************************************************/
const char* UIObject::GetID()
{
	return m_ID;
}

/*****************************************************************************************/
void UIObject::SetPosition(Vector2D &position)
{
	SetPosition(position.x,position.y);
}

/*****************************************************************************************/
void UIObject::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

/*****************************************************************************************/
Vector2D UIObject::GetPosition()
{
	return m_position;
}

/*****************************************************************************************/
void UIObject::SetVisible(bool visible)
{
	m_render = visible;
}

/*****************************************************************************************/
void UIObject::SetAssignedState(const char* assignedState)
{
	m_assignedState = assignedState;
}

/*****************************************************************************************/	
const char* UIObject::GetAssignedState()
{
	return m_assignedState;
}
