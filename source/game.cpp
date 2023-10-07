#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "constants.h"
#include "utils.h"

#include <algorithm>
#include <iterator>
#include <iostream>

bool Game::s_GameOver{ false };
const Texture2D* Game::s_pSpriteSheet{ nullptr };
float Game::s_GroundHeight{ .0f };
Rectangle Game::s_GameScreenRect{ 0.f, 0.f, GAME_WIDTH, GAME_HEIGHT };

Game::Game()
{
	// Load the sprite sheet
	s_pSpriteSheet = new const Texture2D{ LoadTexture(SPRITE_SHEET_PATH) };

	// Initialize the Pipes (Fill with 3 pipes)
	m_pPipes.reserve(MAX_PIPES);
	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		m_pPipes.emplace_back(new Pipe{});
	}

	m_pBirds.reserve(POPULATION_SIZE);
	for (int i{ 0 }; i < POPULATION_SIZE; ++i)
	{
		m_pBirds.emplace_back(new Bird{});
	}

	s_GroundHeight = m_GroundSprite.GetScaledHeight();

	ConfigureGameScreen();
	SelectRandomBackground();

	PrintInstructions();
}

Game::~Game()
{
	CleanUp();
}

void Game::Update(float elapsedSec)
{
	HandleInput();

	if (AreAllBirdsDead())
	{
		s_GameOver = true;
		for (const auto& bird : m_pBirds)
		{
			bird->Update(elapsedSec);
		}
		UpdateGeneticAlgorithm();
	}

	MoveGround(elapsedSec, MOVE_SPEED);
	for (auto& bird : m_pBirds)
	{
		bird->UpdateAnimation(elapsedSec);
	}

	// Game started
	if (!m_StartGame) return;

	for (const auto& pipe : m_pPipes)
	{
		pipe->Update(elapsedSec);
	}

	m_ClosestPipeIdx = GetClosestPipeIdx();
	m_NextPipeIdx = GetNextPipeIdx();

	if (m_pPipes[m_ClosestPipeIdx]->IsOffScreen())
	{
		m_pPipes[m_ClosestPipeIdx]->SetPosX(m_pPipes[(m_ClosestPipeIdx - 1 + MAX_PIPES) % MAX_PIPES]->GetPosition().x);
		m_pPipes[m_ClosestPipeIdx]->AddPosX(PIPE_HORIZONTAL_GAP + m_pPipes[m_ClosestPipeIdx]->GetWidth());
	}

	for (const auto& bird : m_pBirds)
	{
		bird->Update(elapsedSec);
		bird->CalculateBirdPipeHeightDelta(m_pPipes[m_NextPipeIdx]->GetPipeGapCenter());

		if (bird->IsDead()) continue;

		if (m_pPipes[m_ClosestPipeIdx]->HasPassed(bird->GetHitCircleCenter()))
		{
			++m_Score;
		}
	}

	HandleCollision();
}

void Game::Draw() const
{
	BeginDrawing();
	ClearBackground(RED);

	m_BackgroundSprite.Draw(); // Draw first

	for (const auto& pipe : m_pPipes)
	{
		pipe->Draw();
	}

	for (const auto& bird : m_pBirds)
	{
		bird->Draw();
	}

	m_GroundSprite.Draw(); // Draw last

	if (m_ShowFps)
	{
		DrawFPS();
	}

	// Draw the score in the middle of the screen
	if (m_StartGame)
	{
		DrawScore();
	}

	DrawGeneration();
	DrawFitness();
	DrawJumpDelta();

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
	for (auto& pipe : m_pPipes)
	{
		delete pipe;
		pipe = nullptr;
	}

	for (auto& bird : m_pBirds)
	{
		delete bird;
		bird = nullptr;
	}

	UnloadTexture(*s_pSpriteSheet);
	delete s_pSpriteSheet;
	s_pSpriteSheet = nullptr;
}

void Game::DrawFPS() const
{
	const int fontSize{ static_cast<int>(5.f * Sprite::GetGlobalScale().x) };
	DrawText
	(
		TextFormat("FPS: %i", GetFPS()),
		static_cast<int>(m_BackgroundSprite.GetPosition().x) + m_LogTextPadding,
		m_LogTextPadding,
		fontSize,
		BLACK
	);
}

void Game::DrawGeneration() const
{
	const int fontSize{ static_cast<int>(5.f * Sprite::GetGlobalScale().x) };
	DrawText
	(
		TextFormat("Generation: #%i", m_Generation),
		static_cast<int>(m_BackgroundSprite.GetPosition().x) + m_LogTextPadding,
		m_LogTextPadding + fontSize * 2,
		fontSize,
		BLACK
	);
}

void Game::DrawFitness() const
{
	const int fontSize{ static_cast<int>(5.f * Sprite::GetGlobalScale().x) };
	DrawText
	(
		TextFormat("Fitness: %.2f", m_BestFitness),
		static_cast<int>(m_BackgroundSprite.GetPosition().x) + m_LogTextPadding,
		m_LogTextPadding + fontSize * 3,
		fontSize,
		BLACK
	);
}

void Game::DrawJumpDelta() const
{
	const int fontSize{ static_cast<int>(5.f * Sprite::GetGlobalScale().x) };
	DrawText
	(
		TextFormat("Jump at Delta: %.2f", m_BestJumpDelta),
		static_cast<int>(m_BackgroundSprite.GetPosition().x) + m_LogTextPadding,
		m_LogTextPadding + fontSize * 4,
		fontSize,
		BLACK
	);
}

void Game::DrawScore() const
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

	if (s_GameOver) return;

	// Flap the bird
	if (IsKeyPressed(KEY_SPACE))
	{
		// Start the game on the first flap
		m_StartGame = true;
	}
}

void Game::HandleCollision()
{
	if (m_IsOnGround) return;

	for (const auto& bird : m_pBirds)
	{
		if (CheckCollisionCircleRec(bird->GetHitCircleCenter(), bird->GetHitCircleRadius(), m_pPipes[m_ClosestPipeIdx]->GetHitBoxTop()) ||
			CheckCollisionCircleRec(bird->GetHitCircleCenter(), bird->GetHitCircleRadius(), m_pPipes[m_ClosestPipeIdx]->GetHitBoxBottom()))
		{
			bird->SetDeath(true);
		}
	}
}

void Game::RestartGame()
{
	m_BackgroundSprite.ResetSrcRect();
	SelectRandomBackground();
	for (const auto& bird : m_pBirds)
	{
		bird->Initialize();
	}

	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		m_pPipes[i]->Initialize(PIPE_HORIZONTAL_GAP * static_cast<float>(i) + m_pPipes[i]->GetWidth() * static_cast<float>(i));
	}

	m_Score = 0;
	s_GameOver = false;
	m_IsOnGround = false;
}

int Game::GetClosestPipeIdx() const
{
	// Get the index of the closest pipe to the bird (smallest x value)
	int closestPipeIdx{ 0 };
	for (int i{ 0 }; i < MAX_PIPES; ++i)
	{
		if (m_pPipes[i]->GetPosition().x < m_pPipes[closestPipeIdx]->GetPosition().x)
		{
			closestPipeIdx = i;
		}
	}
	return closestPipeIdx;
}
bool Game::AreAllBirdsDead() const
{
	return std::none_of(m_pBirds.begin(), m_pBirds.end(), [](const auto& bird) { return !bird->IsDead(); });
}

void Game::PrintInstructions() const
{
	std::cout << "\n====== Controls ======\n"
			<< "Press [SPACE] to start the game\n"
			<< "Press [F] to toggle FPS\n"
			<< "Press [R] to restart the game\n\n";
}

void Game::UpdateGeneticAlgorithm()
{
	// Update the generation
	++m_Generation;

	SortBirdsVector();

	// Select the fittest birds
	std::vector<Bird*> pBestBirds;
	pBestBirds.reserve(SAMPLE_SIZE);

	// Print the best birds data
	std::cout << "Generation: #" << m_Generation << '\n'
		<< "Fitness: " << m_BestFitness << '\n'
		<< "Jump at Delta: " << m_BestJumpDelta << "\n\n";

	std::copy_n(m_pBirds.begin(), SAMPLE_SIZE, std::back_inserter(pBestBirds));

	// Mutate the best birds
	for (size_t i{ 1 }; i < m_pBirds.size(); ++i)
	{
		m_pBirds[i]->SetJumpAtDelta(pBestBirds[utils::RandomInt(0, SAMPLE_SIZE - 1)]->GetJumpAtDelta() * utils::RandomFloat(1.f - MUTATION_RATE, 1.f + MUTATION_RATE));
	}

	RestartGame();
}

void Game::SortBirdsVector()
{
	// Sort the birds in descending order of fitness
	std::sort(m_pBirds.begin(), m_pBirds.end(), [](const auto& lhs, const auto& rhs)
		{
			return lhs->GetFitness() > rhs->GetFitness();
		});

	m_BestFitness = m_pBirds.front()->GetFitness();
	m_BestJumpDelta = m_pBirds.front()->GetJumpAtDelta();
}

int Game::GetNextPipeIdx() const
{
	// Returns the next pipe the bird needs to go through
	for (const auto& bird : m_pBirds)
	{
		if (bird->IsDead()) continue;

		// If the birds x position is greater than the pipes x position + the pipes width closestPipeIdx
		// the return the index of the next pipe
		if (m_pPipes[m_NextPipeIdx]->GetPosition().x + m_pPipes[m_NextPipeIdx]->GetScaledWidth() < bird->GetPosition().x)
		{
			return (m_NextPipeIdx + 1) % MAX_PIPES;
		}
		return m_NextPipeIdx;
	}

	return 0;
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

	if (!m_StartGame)
	{
		for (const auto& bird : m_pBirds)
		{
			bird->Initialize();
		}

		for (int i{ 0 }; i < MAX_PIPES; ++i)
		{
			m_pPipes[i]->Initialize(PIPE_HORIZONTAL_GAP * static_cast<float>(i) + m_pPipes[i]->GetWidth() * static_cast<float>(i));
		}
	}
	else
	{
		for (const auto& bird : m_pBirds)
		{
			bird->RefreshPosition();
		}
		for (const auto& pipe : m_pPipes)
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
