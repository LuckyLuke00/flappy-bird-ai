#include "raylib.h"

#include "game.h"

#include "constants.h"

const Texture2D* Game::s_pSpriteSheet{ nullptr };

Game::Game()
{
	// Load the sprite sheet
	s_pSpriteSheet = new const Texture2D{ LoadTexture(SPRITE_SHEET_PATH) };

	// Calculate the global scale based on the screen size and the background sprite size, as the background
	// Should cover the whole height of the screen.
	const float scaleToScreenHeight{ static_cast<float>(GetScreenHeight()) / m_Background.GetSrcRect().height };
	Sprite::SetGlobalScale({ scaleToScreenHeight, scaleToScreenHeight });

	// Select a random background sprite
	SelectRandomBackground();

	// Set the ground sprite to the bottom of the screen
	m_Ground.SetPosition({ .0f, static_cast<float>(GetScreenHeight()) - m_Ground.GetHeight() });

	m_Bird.CenterOnScreen();
}

Game::~Game()
{
	CleanUp();
}

void Game::Update(float elapsedSec)
{
	MoveGround(elapsedSec, 250.f);
}

void Game::Draw() const
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	m_Background.Draw(); // Draw first

	m_Bird.Draw();

	m_Ground.Draw(); // Draw last

	if (m_ShowFps)
	{
		DrawFPS(10, 10);
	}

	EndDrawing();
}

void Game::ToggleFps()
{
	m_ShowFps = !m_ShowFps;
}

void Game::CleanUp()
{
	UnloadTexture(*s_pSpriteSheet);
	delete s_pSpriteSheet;
	s_pSpriteSheet = nullptr;
}

void Game::SelectRandomBackground()
{
	const float random{ static_cast<float>(GetRandomValue(0, 1)) };

	// Modify the source rectangle of the background sprite to select a random background
	Rectangle srcRect{ m_Background.GetSrcRect() };
	const float offset{ srcRect.width + 2.f }; // +2.f to account for the 2px gap between sprites
	srcRect.x = offset * random;

	m_Background.SetSrcRect(srcRect);
}

void Game::MoveGround(float elapsedSec, const float speed)
{
	// Move the ground sprite source rectangle to the left
	const float offset{ m_Ground.GetWidth() - m_Background.GetWidth() };
	Vector2 pos{ m_Ground.GetPosition() };
	pos.x -= speed * elapsedSec;

	m_Ground.SetPosition(pos);

	if (m_Ground.GetPosition().x < -offset)
	{
		// If the ground sprite is completely off screen, move it to the right of the screen
		pos.x += offset;
		m_Ground.SetPosition(pos);
	}
}
