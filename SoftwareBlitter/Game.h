#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

class Fish;
class SpriteInstance;

class Game
{
public:
	Game();

	~Game();

	void Update(float elapsedTime);

	void Render();

private:

	void DrawBackground();

	void DrawFish();

	void DrawUI();

	std::vector<Fish*> m_fish;

	SpriteInstance* m_background;
	SpriteInstance* m_title;

	const unsigned int c_maxFish = 30;

	float m_fishSpawnTimer;
};

#endif

