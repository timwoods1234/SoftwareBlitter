#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "Library/MyLinkedList.h"

#include "Library/Singleton.h"

#include "Library/MyString.h"

#include "UIObject.h"

#define BLANK 10
#define ENDOFLINE 59
#define SPACE 32

class UIManager : public Singleton<UIManager>
{
public:
	UIManager(void);

	~UIManager(void);

	void LoadFile(const char* fileName);

	const char* Scanfile(char *Source,int &counter);

	void AddObject(UIObject* object);

	UIObject* GetUIObject(int index);

	UIObject* GetUIObject(const char* ID);

	void Render();

	void Update(float elapsedTime);

	void OnLostDevice();

	void SetCurrentUI(const char* state);

private:

	MyLinkedList<UIObject*> m_objects;

	MyString m_state;
};

#endif // _UIMANAGER_H_
