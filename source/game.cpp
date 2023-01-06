#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "constants.h"

const Texture2D* Game::s_pSpriteSheet{ nullptr };
Rectangle Game::s_GameScreenRect{ 0.f, 0.f, GAME_WIDTH, GAME_HEIGHT };

Game::Game()
{
	// Load the sprite sheet
	s_pSpriteSheet = new const Texture2D{ LoadTexture(SPRITE_SHEET_PATH) };

	// Initialize the Pipes (Fill with 3 pipes)
	m_Pipes.reserve(MAX_PIPES);
	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		m_Pipes.emplace_back(new Pipe{});
	}

	m_Birds.reserve(MAX_BIRDS);
	for (int i{ 0 }; i < MAX_BIRDS; ++i)
	{
		m_Birds.emplace_back(new Bird{});
	}

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

	if (m_IsOnGround)
	{
		for (const auto& bird : m_Birds)
		{
			bird->RotateBird(elapsedSec);
		}
		return;
	}

	if (m_GameOver)
	{
		// Keep rotating the bird when the game is over, so the bird will always face downwards
		HandleCollision();
		for (const auto& bird : m_Birds)
		{
			bird->Update(elapsedSec);
		}
		return;
	}

	MoveGround(elapsedSec, MOVE_SPEED);
	for (auto& bird : m_Birds)
	{
		bird->UpdateAnimation(elapsedSec);
	}

	// Game started
	if (!m_StartGame) return;
	for (const auto& bird : m_Birds)
	{
		bird->Update(elapsedSec);
	}

	for (const auto& pipe : m_Pipes)
	{
		pipe->Update(elapsedSec);
	}

	m_ClosestPipeIdx = GetClosestPipeIdx();

	for (const auto& bird : m_Birds)
	{
		if (m_Pipes[m_ClosestPipeIdx]->HasPassed(bird->GetHitCircleCenter()))
		{
			++m_Score;
		}
	}

	if (m_Pipes[m_ClosestPipeIdx]->IsOffScreen())
	{
		m_Pipes[m_ClosestPipeIdx]->SetPosX(m_Pipes[(m_ClosestPipeIdx - 1 + MAX_PIPES) % MAX_PIPES]->GetPosition().x);
		m_Pipes[m_ClosestPipeIdx]->AddPosX(PIPE_HORIZONTAL_GAP + m_Pipes[m_ClosestPipeIdx]->GetWidth());
	}

	HandleCollision();
}

void Game::Draw() const
{
	BeginDrawing();
	ClearBackground(RED);

	m_BackgroundSprite.Draw(); // Draw first

	for (const auto& pipe : m_Pipes)
	{
		pipe->Draw();
	}

	for (const auto& bird : m_Birds)
	{
		bird->Draw();
	}

	m_GroundSprite.Draw(); // Draw last

	if (m_ShowFps)
	{
		constexpr int padding{ 20 };
		const int fontSize{ static_cast<int>(5.f * Sprite::GetGlobalScale().x) };
		DrawText
		(
			TextFormat("FPS: %i", GetFPS()),
			static_cast<int>(m_BackgroundSprite.GetPosition().x + padding),
			padding,
			fontSize,
			BLACK
		);
	}

	// Draw the score in the middle of the screen
	if (m_StartGame)
	{
		const int fontSize{ static_cast<int>(15.f * Sprite::GetGlobalScale().x) };
		const int scoreWidth{ MeasureText(TextFormat("%i", m_Score), fontSize) };
		DrawText
		(
			TextFormat("%i", m_Score),
			GetScreenWidth() / 2 - scoreWidth / 2,
			GetScreenHeight() / 6 - fontSize / 2,
			fontSize,
			BLACK
		);
	}

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
	for (auto& pipe : m_Pipes)
	{
		delete pipe;
		pipe = nullptr;
	}

	for (auto& bird : m_Birds)
	{
		delete bird;
		bird = nullptr;
	}

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

	if (IsKeyPressed(KEY_R))
	{
		RestartGame();
	}

	if (m_GameOver) return;

	// Flap the bird
	if (IsKeyPressed(KEY_SPACE))
	{
		// Start the game on the first flap
		m_StartGame = true;
		for (const auto& bird : m_Birds)
		{
			bird->Flap();
		}
	}
}

void Game::HandleCollision()
{
	if (m_IsOnGround) return;

	for (const auto& bird : m_Birds)
	{
		if (bird->GetPosition().y + bird->GetScaledHeight() > m_GroundSprite.GetPosition().y)
		{
			m_GameOver = true;
			m_IsOnGround = true;
			return;
		}
	}

	// Check collision with pipes
	for (const auto& bird : m_Birds)
	{
		if (CheckCollisionCircleRec(bird->GetHitCircleCenter(), bird->GetHitCircleRadius(), m_Pipes[m_ClosestPipeIdx]->GetHitBoxTop()) ||
			CheckCollisionCircleRec(bird->GetHitCircleCenter(), bird->GetHitCircleRadius(), m_Pipes[m_ClosestPipeIdx]->GetHitBoxBottom()))
		{
			m_GameOver = true;
			return;
		}
	}
}

void Game::RestartGame()
{
	m_BackgroundSprite.ResetSrcRect();
	SelectRandomBackground();
	for (const auto& bird : m_Birds)
	{
		bird->Initialize();
	}

	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		m_Pipes[i]->Initialize(PIPE_HORIZONTAL_GAP * static_cast<float>(i) + m_Pipes[i]->GetWidth() * static_cast<float>(i));
	}

	m_Score = 0;
	m_StartGame = false;
	m_GameOver = false;
	m_IsOnGround = false;
}

int Game::GetClosestPipeIdx() const
{
	// Get the index of the closest pipe to the bird (smallest x value)
	int closestPipeIdx{ 0 };
	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		if (m_Pipes[i]->GetPosition().x < m_Pipes[closestPipeIdx]->GetPosition().x)
		{
			closestPipeIdx = i;
		}
	}
	return closestPipeIdx;
}

void Game::ConfigureGameScreen()
{
	// Calculate the global scale based on the screen size and the background sprite size, as the background
	// Should cover the whole height of the screen.
	const float scaleToScreenHeight{ static_cast<float>(GetScreenHeight()) / m_BackgroundSprite.GetSrcRect().height };
	Sprite::SetGlobalScale({ scaleToScreenHeight, scaleToScreenHeight });

	// Center the sprites
	m_BackgroundSprite.CenterOnScreen();
	s_GameScreenRect = { m_BackgroundSprite.GetPosition().x, m_BackgroundSprite.GetPosition().y, m_BackgroundSprite.GetScaledWidth(), m_BackgroundSprite.GetScaledHeight() };

	// Set the ground sprite to the bottom of the screen
	m_GroundSprite.SetPosition({ m_BackgroundSprite.GetPosition().x, static_cast<float>(GetScreenHeight()) - m_GroundSprite.GetScaledHeight() });

	Pipe::SetGroundHeight(m_GroundSprite.GetHeight());

	if (!m_StartGame)
	{
		for (const auto& bird : m_Birds)
		{
			bird->Initialize();
		}

		for (int i{ 0 }; i < MAX_PIPES; ++i)
		{
			m_Pipes[i]->Initialize(PIPE_HORIZONTAL_GAP * static_cast<float>(i) + m_Pipes[i]->GetWidth() * static_cast<float>(i));
		}
	}
	else
	{
		for (const auto& bird : m_Birds)
		{
			bird->RefreshPosition();
		}
		for (const auto& pipe : m_Pipes)
		{
			pipe->RefreshPosition();
		}
	}

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
		width = static_cast<int>(floorf(m_BackgroundSprite.GetScaledWidth()));
	}
	else
	{
		// Round down
		x = static_cast<int>(floorf(m_BackgroundSprite.GetPosition().x));

		// Do the opposite for the width, to prevent the background color from showing
		width = static_cast<int>(ceilf(m_BackgroundSprite.GetScaledWidth()));
	}

	BeginScissorMode
	(
		x,
		static_cast<int>(m_BackgroundSprite.GetPosition().y),
		width,
		static_cast<int>(m_BackgroundSprite.GetScaledHeight())
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
	const float offset{ m_GroundSprite.GetScaledWidth() - m_BackgroundSprite.GetScaledWidth() };
	m_GroundSprite.AddPosX(-speed * elapsedSec);

	if (m_GroundSprite.GetPosition().x < m_BackgroundSprite.GetPosition().x - offset)
	{
		// If the ground sprite is completely off screen, move it to the right of the screen
		m_GroundSprite.SetPosition({ m_GroundSprite.GetPosition().x + offset, m_GroundSprite.GetPosition().y });
	}
}
