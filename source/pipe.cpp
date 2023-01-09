#include "pipe.h"
#include "game.h"
#include "constants.h"
#include "utils.h"

void Pipe::Draw() const
{
	m_TopPipeSprite.Draw();
	m_BottomPipeSprite.Draw();
}

void Pipe::Update(float elapsedSec)
{
	UpdatePosPercent();

	// Move both pipes to the left
	m_TopPipeSprite.AddPosX(-MOVE_SPEED * elapsedSec);
	m_BottomPipeSprite.AddPosX(-MOVE_SPEED * elapsedSec);

	UpdateHitBoxes();
}

Vector2 Pipe::GetPipeGapCenter() const
{
	// Returns the center of the gap between the pipes
	return Vector2
	{
		m_BottomPipeSprite.GetPosition().x + m_BottomPipeSprite.GetScaledWidth() * .5f,
		m_BottomPipeSprite.GetPosition().y - PIPE_VERTICAL_GAP * Sprite::GetGlobalScale().x * .5f
	};
}

void Pipe::Initialize(const float offsetX)
{
	SetToRightOfScreen();

	// Offset them by once the game screen width
	m_TopPipeSprite.AddPosX(GAME_WIDTH + offsetX);
	m_BottomPipeSprite.AddPosX(GAME_WIDTH + offsetX);

	m_Passed = false;

	AddVerticalGap();

	SetRandomHeight();
	UpdatePosPercent();
}

bool Pipe::IsOffScreen() const
{
	return static_cast<float>(GetScreenWidth()) * m_PosPercentX + m_TopPipeSprite.GetScaledWidth() < Game::GetGameScreenRect().x;
}

bool Pipe::HasPassed(const Vector2& birdPos)
{
	if (birdPos.x < static_cast<float>(GetScreenWidth()) * m_PosPercentX + m_TopPipeSprite.GetScaledWidth() * .5f)
	{
		m_Passed = false;
		return m_Passed;
	}

	if (m_Passed) return false;

	m_Passed = true;
	return m_Passed;
}

void Pipe::RefreshPosition()
{
	m_TopPipeSprite.SetPosition({ static_cast<float>(GetScreenWidth()) * m_PosPercentX, static_cast<float>(GetScreenHeight()) * m_PosPercentYTop });
	m_BottomPipeSprite.SetPosition({ static_cast<float>(GetScreenWidth()) * m_PosPercentX, static_cast<float>(GetScreenHeight()) * m_PosPercentYBottom });
}

void Pipe::SetRandomHeight()
{
	// Set a random height for the top pipe
	const float randomHeight{ utils::RandomFloat(-MAX_PIPE_HEIGHT * .5f, MAX_PIPE_HEIGHT * .5f) - Game::GetGroundHeight() };

	// Set the height of the top pipe
	m_TopPipeSprite.AddPosY(randomHeight * .5f);
	m_BottomPipeSprite.AddPosY(randomHeight * .5f);
}

void Pipe::UpdatePosPercent()
{
	m_PosPercentX = m_TopPipeSprite.GetPosition().x / static_cast<float>(GetScreenWidth());
	m_PosPercentYTop = m_TopPipeSprite.GetPosition().y / static_cast<float>(GetScreenHeight());
	m_PosPercentYBottom = m_BottomPipeSprite.GetPosition().y / static_cast<float>(GetScreenHeight());
}

void Pipe::SetToRightOfScreen()
{
	// Center the pipes on the screen
	m_TopPipeSprite.CenterOnScreen();
	m_BottomPipeSprite.CenterOnScreen();

	// Set the position of the pipes
	// so that the bottom of the top pip touches the top of the bottom pipe
	m_TopPipeSprite.SetPosY(m_TopPipeSprite.GetPosition().y - m_TopPipeSprite.GetScaledHeight() * .5f);
	m_BottomPipeSprite.SetPosY(m_BottomPipeSprite.GetPosition().y + m_BottomPipeSprite.GetScaledHeight() * .5f);

	// Set the position of the pipes to the right of the game screen
	m_TopPipeSprite.SetPosX(m_TopPipeSprite.GetPosition().x + Game::GetGameScreenRect().width * .5f + m_TopPipeSprite.GetScaledWidth() * .5f);
	m_BottomPipeSprite.SetPosX(m_BottomPipeSprite.GetPosition().x + Game::GetGameScreenRect().width * .5f + m_BottomPipeSprite.GetScaledWidth() * .5f);
}

void Pipe::AddVerticalGap()
{
	// Add a vertical gap between the pipes
	m_TopPipeSprite.AddPosY(-PIPE_VERTICAL_GAP * .5f);
	m_BottomPipeSprite.AddPosY(PIPE_VERTICAL_GAP * .5f);
}

void Pipe::UpdateHitBoxes()
{
	// Top pipe
	m_HitBoxTop.x = m_TopPipeSprite.GetPosition().x;
	m_HitBoxTop.y = m_TopPipeSprite.GetPosition().y;
	m_HitBoxTop.width = m_TopPipeSprite.GetScaledWidth();
	m_HitBoxTop.height = m_TopPipeSprite.GetScaledHeight();

	// Bottom pipe
	m_HitBoxBottom.x = m_BottomPipeSprite.GetPosition().x;
	m_HitBoxBottom.y = m_BottomPipeSprite.GetPosition().y;
	m_HitBoxBottom.width = m_BottomPipeSprite.GetScaledWidth();
	m_HitBoxBottom.height = m_BottomPipeSprite.GetScaledHeight();
}
