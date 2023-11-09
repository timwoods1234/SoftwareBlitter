#include "Library/SoundManager.h"
#include "Library/GameStateManager.h"
#include "Library/Vector2D.h"
#include "Library/InputManager.h"

#include "GameState_Fighting.h"
#include "Game.h"
#include "Settings.h"
#include "SoftSpriteAnimated.h"
#include "UIManager.h"
#include "Character.h"
#include "SoftSprite.h"

/*****************************************************************************************/
float DistanceBetween(const Vector2D &a, const Vector2D &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;

	return sqrt(dx*dx + dy*dy);
}

/*****************************************************************************************/
GameState_Fighting::GameState_Fighting()
{
	m_stateName = "FIGHTING";

	LoadAssets();
}

/*****************************************************************************************/
GameState_Fighting::~GameState_Fighting()
{
	delete m_background;
	delete m_scoreSprite;

	delete m_playerOne;
	delete m_playerTwo;

	delete m_healthbarP1;
	delete m_healthbarP2;
}

/*****************************************************************************************/
void GameState_Fighting::OnEntry()
{
	UIManager::GetInstance()->SetCurrentUI(GetStateName());

	if (!Game::GetInstance()->IsGameInProgress())
	{
		Game::GetInstance()->SetGameInProgress(true);

		m_playerOne->SetScore(0);
		m_playerTwo->SetScore(0);

		InitGame();
	}
}

/*****************************************************************************************/
void GameState_Fighting::OnExit()
{
}

/*****************************************************************************************/
void GameState_Fighting::Render()
{
	if (m_background != NULL)
	{
		m_background->Render(0, 0);

		m_playerOne->Render();

		m_playerTwo->Render();

		if (m_playerOne->GetHealth() > 0.0f)
		{
			m_healthbarP1->Render(10, 10);
		}

		if (m_playerTwo->GetHealth() > 0.0f)
		{
			m_healthbarP2->Render(Settings::ScreenWidth - m_healthbarP2->GetWidth(), 10);
		}

		for (int i = 0; i < m_playerOne->GetScore(); i++)
		{
			m_scoreSprite->Render(i * 40, 30);
		}

		for (int i = 0; i < m_playerTwo->GetScore(); i++)
		{
			m_scoreSprite->Render(Settings::ScreenWidth - 50 - (i * 40), 30);
		}
	}
}

/*****************************************************************************************/
void GameState_Fighting::Update(float elapsedTime)
{
	InputManager::GetInstance()->Update();

	if (InputManager::GetInstance()->IsKeyClicked(false, DIK_ESCAPE))
	{
		GameStateManager::GetInstance()->SwitchState("FRONTEND");
	}

	if (m_timeSincePlayerWon > 0.0f)
	{
		UIManager::GetInstance()->GetUIObject("Ko")->SetVisible(true);
		UIManager::GetInstance()->GetUIObject("Fight")->SetVisible(false);

		m_timeSincePlayerWon += elapsedTime;

		if (m_timeSincePlayerWon > 2.0f)
		{
			InitGame();
		}
	}
	else
	{
		UIManager::GetInstance()->GetUIObject("Ko")->SetVisible(false);
		UIManager::GetInstance()->GetUIObject("Fight")->SetVisible(true);

		// player 1 KO'ed?
		if (!m_playerOne->Update(elapsedTime))
		{
			m_playerTwo->SetScore(m_playerTwo->GetScore() + 1);
			m_timeSincePlayerWon = 0.1f;
		}

		// player 2 KO'ed?
		if (!m_playerTwo->Update(elapsedTime))
		{
			m_playerOne->SetScore(m_playerOne->GetScore() + 1);
			m_timeSincePlayerWon = 0.1f;
		}

		CollisionDetection();
	}
}

/*****************************************************************************************/
void GameState_Fighting::OnLostDevice()
{
	SoundManager::GetInstance()->OnLostDevice();
}

/*****************************************************************************************/
void GameState_Fighting::InitGame()
{
	// check score (for winner)
	if (m_playerOne->GetScore() > Settings::MAX_ROUNDS)
	{
		Game::GetInstance()->SetGameInProgress(false);

		SoundManager::GetInstance()->PlaySoundFile("win", "Assets/Sound/win.wav", false, false, Settings::Volume);

		GameStateManager::GetInstance()->SwitchState("GAMEOVER");
	}
	else if (m_playerTwo->GetScore() > Settings::MAX_ROUNDS)
	{
		Game::GetInstance()->SetGameInProgress(false);

		SoundManager::GetInstance()->PlaySoundFile("lose", "Assets/Sound/lose.wav", false, false, Settings::Volume);
		GameStateManager::GetInstance()->SwitchState("GAMEOVER");
	}

	if (Game::GetInstance()->IsGameInProgress())
	{
		m_playerOne->Reset();
		m_playerTwo->Reset();

		m_playerOne->SetPosition(Vector2D(75.0f, Settings::ScreenHeight - 140.0f));
		m_playerTwo->SetPosition(Vector2D(Settings::ScreenWidth - 125.0f, Settings::ScreenHeight - 140.0f));
		m_playerTwo->SetFlipX(true);

		m_timeSincePlayerWon = 0.0f;

		m_healthbarP1->SetCurrentAnimation(0);
		m_healthbarP2->SetCurrentAnimation(0);

		SoundManager::GetInstance()->PlaySoundFile("fight", "Assets/Sound/fight.wav", false, false, Settings::Volume);
	}
}

/*****************************************************************************************/
void GameState_Fighting::CollisionDetection()
{
	if (DistanceBetween(m_playerOne->GetPosition(), m_playerTwo->GetPosition()) < (m_playerOne->GetRadius() + m_playerTwo->GetRadius()))
	{
		// TODO: make sure they're facing the right way!
		if (m_playerOne->IsAttacking())
		{
			if (m_playerTwo->GetHit(1.0f))
			{
				m_healthbarP2->IncrementFrame();
			}
		}

		if (m_playerTwo->IsAttacking())
		{
			if (m_playerOne->GetHit(1.0f))
			{
				m_healthbarP1->IncrementFrame();
			}
		}
	}
}

/*****************************************************************************************/
void GameState_Fighting::LoadAssets()
{
	m_background = new SoftSprite();
	m_background->LoadSprite("Assets/background.bmp");

	m_scoreSprite = new SoftSprite();
	m_scoreSprite->LoadSprite("Assets/score.bmp");
	m_scoreSprite->SetDefaultColorKey();
	m_scoreSprite->GetBMP()->CompressPalette(30);

	m_playerOne = new Character();
	m_playerOne->EnableAI(false);

	m_playerTwo = new Character();
	m_playerTwo->EnableAI(true);

	m_playerTwo->GetSprite()->SwapColour(13693168, 5074419);

	m_healthbarP1 = new SoftSpriteAnimated();
	m_healthbarP1->LoadSprite("Assets/Health_GFX.txt", "Assets/Health_Def.txt");
	m_healthbarP1->SetDefaultColorKey();

	m_healthbarP2 = new SoftSpriteAnimated();
	m_healthbarP2->LoadSprite("Assets/Health_GFX.txt", "Assets/Health_Def.txt");
	m_healthbarP2->SetDefaultColorKey();
	m_healthbarP2->SetFlipX(true);
}
