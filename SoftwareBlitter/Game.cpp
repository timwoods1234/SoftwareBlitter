
#include "Game.h"
#include "Fish.h"
#include "SpriteInstance.h"
#include "SoftSprite.h"
#include "SoftwareRenderer.h"
#include "BlendMode.h"

/*****************************************************************************************/
Game::Game()
{
	m_fishSpawnTimer = 0.f;

	m_fish.reserve(c_maxFish);

	m_background = new SpriteInstance("Assets/Background.bmp");
	m_background->SetPosition(Vector2D::Zero());
}

/*****************************************************************************************/
Game::~Game()
{
	delete m_background;

	for (unsigned int index = 0; index < m_fish.size(); index++)
	{
		delete m_fish[index];
	}
}

/*****************************************************************************************/
void Game::Update(float elapsedTime)
{
	for (unsigned int index = 0; index < m_fish.size(); index++)
	{
		m_fish[index]->Update(elapsedTime);
	}

	m_fishSpawnTimer -= elapsedTime;

	if (m_fishSpawnTimer <= 0.f && m_fish.size() < c_maxFish)
	{
		Fish* newFish = new Fish();

		m_fish.push_back(newFish);

		m_fishSpawnTimer = 0.05f;
	}
}

/*****************************************************************************************/
void Game::Render()
{
	DrawFish();

	DrawBackground();
}

/*****************************************************************************************/
void Game::DrawBackground()
{
	// draw this last with ALWAYS_FAIL_DEPTH to write minimum amount of pixels
	SoftwareRenderer::GetCurrentInstance()->SetBlendMode(ALWAYS_FAIL_DEPTH);
	m_background->Render();
}

/*****************************************************************************************/
void Game::DrawFish()
{
	SoftwareRenderer::GetCurrentInstance()->SetBlendMode(COMPARE_DEPTH);

	for (unsigned int index = 0; index < m_fish.size(); index++)
	{
		m_fish[index]->Render();
	}
}
