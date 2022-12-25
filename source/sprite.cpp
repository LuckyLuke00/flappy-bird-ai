#include "sprite.h"
#include "Game.h"

Vector2 Sprite::s_GlobalScale{ 1.f, 1.f };

Sprite::Sprite(const Rectangle& srcRect, const Vector2& pos, const Vector2& scale, const float rotation)
	: m_Rotation{ rotation },
	m_SrcRect{ srcRect },
	m_Position{ pos },
	m_Scale{ scale }
{
}

Sprite::~Sprite()
{
}

void Sprite::Draw() const
{
	DrawTexturePro
	(
		*Game::GetSpriteSheet(),
		m_SrcRect,
		{
			m_Position.x,
			m_Position.y,
			m_SrcRect.width * (m_Scale.x * s_GlobalScale.x),
			m_SrcRect.height * (m_Scale.y * s_GlobalScale.y)
		},
		{ .0f, .0f },
		m_Rotation,
		WHITE
	);
}

void Sprite::FitScreenHeight()
{
	// Set's the scale of the sprite to fit the screen height
	m_Scale.y = static_cast<float>(GetScreenHeight()) / m_SrcRect.height;
	m_Scale.x = m_Scale.y;
}

void Sprite::FitScreenWidth()
{
	// Set's the scale of the sprite to fit the screen width
	m_Scale.x = static_cast<float>(GetScreenWidth()) / m_SrcRect.width;
	m_Scale.y = m_Scale.x;
}

void Sprite::CenterOnScreen()
{
	// Centers the sprite on the screen
	m_Position.x = static_cast<float>(GetScreenWidth()) * .5f - (m_SrcRect.width * m_Scale.x * s_GlobalScale.x) * .5f;
	m_Position.y = static_cast<float>(GetScreenHeight()) * .5f - (m_SrcRect.height * m_Scale.y * s_GlobalScale.y) * .5f;
}
