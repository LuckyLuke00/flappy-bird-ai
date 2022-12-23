#ifndef GAME_H
#define GAME_H

#include "structs.h"

class Game
{
public:
	explicit Game();

	Game(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game& operator=(Game&& other) = delete;

	virtual ~Game();

	void Update();
	void Draw() const;

	const Texture2D* GetSpriteSheet() const;
private:
	const Texture2D* m_pSpriteSheet;
	Sprite m_Background;

	void LoadAssets();
	void UnloadAssets();
};

#endif
