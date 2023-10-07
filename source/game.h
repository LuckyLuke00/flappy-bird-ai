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
	static float GetGroundHeight() { return s_GroundHeight; }
	static bool IsGameOver() { return s_GameOver; }
private:
	static bool s_GameOver;
	static float s_GroundHeight;
	bool m_StartGame{ false };
	bool m_IsOnGround{ false };
	int m_Score{ 0 };
	int m_ClosestPipeIdx{ 0 };
	int m_NextPipeIdx{ 0 };
	int m_Generation{ 0 };
	int m_LogTextPadding{ 20 };
	float m_BestFitness{ 0.f };
	float m_BestJumpDelta{ 0.f };

	Sprite m_BackgroundSprite{ { .0f, .0f, 144.f , 256.f } };
	Sprite m_GroundSprite{ { 292.f, .0f, 168.f, 56.f } };

	std::vector<Bird*> m_pBirds{};
	std::vector<Pipe*> m_pPipes;

	// Fps values
	bool m_ShowFps{ false };

	void SelectRandomBackground();
	void MoveGround(float elapsedSec, const float speed);

	void ConfigureGameScreen();
	void CropScreen() const;
	void CleanUp();

	// Text drawing
	void DrawFPS() const;
	void DrawGeneration() const;
	void DrawFitness() const;
	void DrawJumpDelta() const;
	void DrawScore() const;

	void HandleInput();
	void HandleCollision();
	void RestartGame();

	int GetClosestPipeIdx() const;
	int GetNextPipeIdx() const;
	bool AreAllBirdsDead() const;

	void PrintInstructions() const;

	// Function that updates the genetic algorithm
	void UpdateGeneticAlgorithm();
	void SortBirdsVector();

	static const Texture2D* s_pSpriteSheet;
	static Rectangle s_GameScreenRect;
};

#endif
