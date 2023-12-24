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

	std::vector<Fish*> m_fish;

	SpriteInstance* m_background;

	const unsigned int c_maxFish = 200;

	float m_fishSpawnTimer;
};

#endif

