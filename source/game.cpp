#include "game.h"
#include "constants.h"

Game::Game()
{
	LoadAssets();

	// Set the background sprite
	const int random{ GetRandomValue(0, 1) };
	m_Background.sourceRect = { .0f, .0f, 144.f, 256.f };
	m_Background.sourceRect.x += random * (m_Background.sourceRect.width + 2);
	m_Background.ScaleToFitScreen();
}

Game::~Game()
{
	UnloadAssets();
}

void Game::Draw() const
{
	m_Background.Draw(m_pSpriteSheet);
}

const Texture2D* Game::GetSpriteSheet() const
{
	return m_pSpriteSheet;
}

void Game::LoadAssets()
{
	m_pSpriteSheet = new Texture2D{ LoadTexture(SPRITE_SHEET_PATH) };
}

void Game::UnloadAssets()
{
	UnloadTexture(*m_pSpriteSheet);
	delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;
}
