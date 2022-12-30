#include "raylib.h"
#include "raymath.h"

#include "game.h"

#include "constants.h"

#include <iostream>

const Texture2D* Game::s_pSpriteSheet{ nullptr };

Game::Game()
{
	// Load the sprite sheet
	s_pSpriteSheet = new const Texture2D{ LoadTexture(SPRITE_SHEET_PATH) };

	ConfigureGameScreen();

	SelectRandomBackground();
}

Game::~Game()
{
	CleanUp();
}

void Game::Update(float elapsedSec)
{
	HandleInput();

	MoveGround(elapsedSec, 250.f);
	m_Bird.UpdateAnimation(elapsedSec);

	if (!m_StartGame) return;
	m_Bird.Update(elapsedSec);
}

void Game::Draw() const
{
	BeginDrawing();
	ClearBackground(RED);

	m_BackgroundSprite.Draw(); // Draw first

	m_Bird.Draw();

	m_GroundSprite.Draw(); // Draw last

	if (m_ShowFps)
	{
		constexpr int padding{ 20 };
		constexpr int fontSize{ 20 };
		DrawText(TextFormat("FPS: %i", GetFPS()), static_cast<int>(m_BackgroundSprite.GetPosition().x + padding), padding, fontSize, BLACK);
	}

	//// DEBUG
	//// Draw the two lines in through the center of the screen
	//DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, BLACK);
	//DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), BLACK);

	EndDrawing();
}

void Game::OnWindowResize()
{
	ConfigureGameScreen();
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

void Game::HandleInput()
{
	// Toggle fps
	if (IsKeyPressed(KEY_F))
	{
		ToggleFps();
	}

	// Flap the bird
	if (IsKeyPressed(KEY_SPACE))
	{
		m_StartGame = true;
		m_Bird.Flap();
	}
}

void Game::ConfigureGameScreen()
{
	// Calculate the global scale based on the screen size and the background sprite size, as the background
	// Should cover the whole height of the screen.
	const float scaleToScreenHeight{ static_cast<float>(GetScreenHeight()) / m_BackgroundSprite.GetSrcRect().height };
	Sprite::SetGlobalScale({ scaleToScreenHeight, scaleToScreenHeight });

	// Center the sprites
	m_BackgroundSprite.CenterOnScreen();

	// Set the ground sprite to the bottom of the screen
	m_GroundSprite.SetPosition({ m_BackgroundSprite.GetPosition().x, static_cast<float>(GetScreenHeight()) - m_GroundSprite.GetHeight() });
	m_Bird.Reset();

	CropScreen();
}

void Game::CropScreen() const
{
	// This is the only way to porpperly crop the screen, without having any artifacts.
	// Crop everything to the size of the background sprite
	int x{};
	int width{};

	// Check if the background position.x needs to be rounded up or down
	if (m_BackgroundSprite.GetPosition().x - truncf(m_BackgroundSprite.GetPosition().x) > .5f)
	{
		// Round up
		x = static_cast<int>(ceilf(m_BackgroundSprite.GetPosition().x));

		// Do the opposite for the width, to prevent the background color from showing
		width = static_cast<int>(floorf(m_BackgroundSprite.GetWidth()));
	}
	else
	{
		// Round down
		x = static_cast<int>(floorf(m_BackgroundSprite.GetPosition().x));

		// Do the opposite for the width, to prevent the background color from showing
		width = static_cast<int>(ceilf(m_BackgroundSprite.GetWidth()));
	}

	BeginScissorMode
	(
		x,
		static_cast<int>(m_BackgroundSprite.GetPosition().y),
		width,
		static_cast<int>(m_BackgroundSprite.GetHeight())
	);
}

void Game::SelectRandomBackground()
{
	const float random{ static_cast<float>(GetRandomValue(0, 1)) };

	// Modify the source rectangle of the background sprite to select a random background
	Rectangle srcRect{ m_BackgroundSprite.GetSrcRect() };
	const float offset{ srcRect.width + 2.f }; // +2.f to account for the 2px gap between sprites
	srcRect.x += offset * random;

	m_BackgroundSprite.SetSrcRectPos(srcRect.x, srcRect.y);
}

void Game::MoveGround(float elapsedSec, const float speed)
{
	// Move the ground sprite source rectangle to the left
	const float offset{ m_GroundSprite.GetWidth() - m_BackgroundSprite.GetWidth() };
	Vector2 pos{ m_GroundSprite.GetPosition() };
	pos.x -= speed * elapsedSec;

	m_GroundSprite.SetPosition(pos);

	if (m_GroundSprite.GetPosition().x < m_BackgroundSprite.GetPosition().x - offset)
	{
		// If the ground sprite is completely off screen, move it to the right of the screen
		pos.x += offset;
		m_GroundSprite.SetPosition(pos);
	}
}
