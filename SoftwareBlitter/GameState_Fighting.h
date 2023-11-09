#ifndef __GAMESTATE_FIGHTING_H__
#define __GAMESTATE_FIGHTING_H__

#ifndef __GAMESTATE_H__
#include "Library/GameState.h"
#endif

class SoftSprite;
class Character;
class SoftSpriteAnimated;

class GameState_Fighting : public GameState
{
public:

	GameState_Fighting();

	~GameState_Fighting();

	virtual void OnEntry();

	virtual void OnExit();

	virtual void Render();

	virtual void Update(float elapsedTime);

	virtual void OnLostDevice();

private:

	void InitGame();	// called at the start of each round

	void CollisionDetection();

	void LoadAssets();

	SoftSprite* m_background;

	SoftSprite* m_scoreSprite;

	BaseCharacter* m_playerOne;

	BaseCharacter* m_playerTwo;

	SoftSpriteAnimated* m_healthbarP1;

	SoftSpriteAnimated* m_healthbarP2;

	float	m_timeSincePlayerWon;	 // delay from player winning until next round
};

#endif
