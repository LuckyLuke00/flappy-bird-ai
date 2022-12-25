#ifndef GAME_H
#define GAME_H

#include "sprite.h"

class Game final
{
public:
	explicit Game();

	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	~Game();

	void Update(float elapsedSec);
	void Draw() const;

	// This function is called when the window is resized, and recalculates all scales and positions.
	void OnWindowResize();

	void ToggleFps();
	static const Texture2D* GetSpriteSheet() { return s_pSpriteSheet; }

private:
	static const Texture2D* s_pSpriteSheet;
	Sprite m_Background{ { .0f, .0f, 144.f , 256.f } };
	Sprite m_Ground{ { 292.f, .0f, 168.f, 56.f } };
	Sprite m_Bird{ { 3.f, 491.f, 17.f, 12.f } }; // Temporary

	// Fps values
	bool m_ShowFps{ false };

	void SelectRandomBackground();
	void MoveGround(float elapsedSec, const float speed);

	void ConfigureGameScreen();
	void CropScreen() const;
	void CleanUp();
};

#endif
