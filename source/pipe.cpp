#include "pipe.h"
#include "game.h"
#include "constants.h"

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
}

void Pipe::Initialize(const float offsetX)
{
	SetToRightOfScreen();

	// Offset them by once the game screen width
	m_TopPipeSprite.AddPosX(GAME_WIDTH + offsetX);
	m_BottomPipeSprite.AddPosX(GAME_WIDTH + offsetX);

	AddVerticalGap();

	SetRandomHeight();
	UpdatePosPercent();
}

bool Pipe::IsOffScreen() const
{
	return  static_cast<float>(GetScreenWidth()) * m_PosPercentX + m_TopPipeSprite.GetScaledWidth() < Game::GetGameScreenRect().x;
}

void Pipe::RefreshPosition()
{
	m_TopPipeSprite.SetPosition({ static_cast<float>(GetScreenWidth()) * m_PosPercentX, static_cast<float>(GetScreenHeight()) * m_PosPercentYTop });
	m_BottomPipeSprite.SetPosition({ static_cast<float>(GetScreenWidth()) * m_PosPercentX, static_cast<float>(GetScreenHeight()) * m_PosPercentYBottom });
}

void Pipe::SetRandomHeight()
{
	// Set a random height for the top pipe
	// TODO: Take ground height into account (dynamically)
	const float randomHeight{ static_cast<float>(GetRandomValue(-MAX_PIPE_HEIGHT / 2, MAX_PIPE_HEIGHT / 2)) - 56 / 2 };

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
