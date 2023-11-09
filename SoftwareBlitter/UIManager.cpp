#include "UIManager.h"
#include "UISprite.h"
#include "Library/MyFileReader.h"
#include "Library/Vector2D.h"

/*****************************************************************************************/
DefineSingleton(UIManager)

/*****************************************************************************************/
UIManager::UIManager(void)
{
	m_state = "";
}

/*****************************************************************************************/
UIManager::~UIManager(void)
{
	for(int i = 0; i < m_objects.GetNumEntries(); i++)
	{
		delete m_objects[i];
	}
}

/*****************************************************************************************/
void UIManager::LoadFile(const char* fileName)
{
	char* temp;
	MyFileReader* reader = new MyFileReader();

	if(reader->Exists(fileName))
	{
		reader->OpenFile(fileName,"r");
		
		reader->ReadBuffer(temp);

		reader->CloseFile();

		delete reader;
	}
	else
	{
		delete reader;
		assert(false);
	}
	int counter = 0;
	int numberOfCharacters = atoi(Scanfile(temp,counter));

	for (int i = 0; i < numberOfCharacters; i++)
	{
		MyString Details;
		MyString Type;
		Type = Scanfile(temp,counter);

		if(Type == "SPRITE")
		{
			m_objects.Insert(new UISprite);
		}

		Details = Scanfile(temp,counter);
		m_objects.GetLast()->GetData()->SetID(Details);

		float x = (float)atoi(Scanfile(temp,counter));
		float y = (float)atoi(Scanfile(temp,counter));
		Vector2D tempVector(x,y);
		m_objects.GetLast()->GetData()->SetPosition(tempVector);
		m_objects.GetLast()->GetData()->SetVisible(false);

		Details = Scanfile(temp,counter);
		m_objects.GetLast()->GetData()->SetAssignedState(Details);

		if(Type == "SPRITE")
		{
			Details = Scanfile(temp,counter);
			static_cast<UISprite*>(m_objects.GetLast()->GetData())->LoadSprite( const_cast<char*>(Details.GetPointer()) );
		}
	}

	free(temp);
}

/*****************************************************************************************/
const char* UIManager::Scanfile(char *Source,int &counter)
{
	MyString tempory = "";
	bool terminate = false;
	while (!terminate)
	{
		if(Source[counter] != SPACE && Source[counter] != ENDOFLINE)
		{
			if(Source[counter] == BLANK)
			{
				counter++;
			}
			else
			{
				tempory.Concatenate(Source[counter]);
				counter++;
			}
		}
		else
		{
			terminate = true;
			counter++;
		}
	}

	return tempory;
}

/*****************************************************************************************/
void UIManager::AddObject(UIObject* object)
{
	m_objects.Insert(object);
}

/*****************************************************************************************/
UIObject* UIManager::GetUIObject(int index)
{
	return m_objects[index];
}

/*****************************************************************************************/
UIObject* UIManager::GetUIObject(const char *ID)
{
	for(int i = 0; i < m_objects.GetNumEntries(); i++)
	{	
		if(strcmp(m_objects[i]->GetID(),ID)==0)
		{
			return m_objects[i];
		}
	}

	return 0;
}

/*****************************************************************************************/
void UIManager::Render()
{
	for (int i = 0; i < m_objects.GetNumEntries(); i++)
	{	
		m_objects[i]->Render();
	}
}

/*****************************************************************************************/
void UIManager::Update(float elapsedTime)
{
	for (int i = 0; i < m_objects.GetNumEntries(); i++)
	{
		m_objects[i]->Update(elapsedTime);
	}	
}

/*****************************************************************************************/
void UIManager::OnLostDevice()
{
	for (int i = 0; i < m_objects.GetNumEntries(); i++)
	{
		m_objects[i]->onLostDevice();
	}
}	

/*****************************************************************************************/
void UIManager::SetCurrentUI(const char* state)
{
	if(strcmp(m_state,state)!=0)
	{
		m_state = state;

		for (int i = 0; i < m_objects.GetNumEntries(); i++)
		{
			if(strcmp(m_objects[i]->GetAssignedState(),state) != 0)
			{
				m_objects[i]->SetVisible(false);
			}
			else
			{
				m_objects[i]->SetVisible(true);
			}
		}
	}
}
