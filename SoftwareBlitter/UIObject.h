#ifndef _UIOBJECT_H_
#define _UIOBJECT_H_

#include "Library/MyString.h"
#include "Library/Vector2D.h"

class UIObject
{
public:
	UIObject(void);

	virtual ~UIObject(void) ;

	virtual void Update(float elapsedTime){};

	virtual void Render(){};

	virtual const char* GetType(){return "OBJECT";};

	virtual void onLostDevice(){};

	virtual void SetValue(const char* string){};

	virtual void SetValue(int number){};

	void SetID(const char* ID);

	const char* GetID();

	void SetPosition(Vector2D &position);

	void SetPosition(float x, float y);

	Vector2D GetPosition();

	void SetAssignedState(const char* assignedState);

	const char* GetAssignedState();

	void SetVisible(bool visible);

protected:
	MyString m_ID;

	Vector2D m_position;

	MyString m_assignedState;

	bool m_render;

};

#endif //_UIOBJECT_H_
