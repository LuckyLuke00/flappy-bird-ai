#ifndef GAME_H
#define GAME_H

#include "bird.h"
#include "pipe.h"

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
	static const Rectangle& GetGameScreenRect() { return s_GameScreenRect; }
private:
	bool m_StartGame{ false };
	bool m_GameOver{ false };
	bool m_IsOnGround{ false };

	Sprite m_BackgroundSprite{ { .0f, .0f, 144.f , 256.f } };
	Sprite m_GroundSprite{ { 292.f, .0f, 168.f, 56.f } };
	Bird m_Bird{};

	std::vector<Pipe*> m_Pipes;

	// Fps values
	bool m_ShowFps{ false };

	void SelectRandomBackground();
	void MoveGround(float elapsedSec, const float speed);

	void ConfigureGameScreen();
	void CropScreen() const;
	void CleanUp();

	void HandleInput();
	void HandleCollision();

	static const Texture2D* s_pSpriteSheet;
	static Rectangle s_GameScreenRect;
};

#endif
