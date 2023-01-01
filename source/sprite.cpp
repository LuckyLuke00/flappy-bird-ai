#include "sprite.h"
#include "Game.h"

#include <iostream>

Vector2 Sprite::s_GlobalScale{ 1.f, 1.f };

Sprite::Sprite(const Rectangle& srcRect, const Vector2& pos, const Vector2& scale, const float rotation)
	: m_Rotation{ rotation },
	m_SrcRect{ srcRect },
	m_Position{ pos },
	m_Scale{ scale }
{
}

void Sprite::Draw() const
{
	// Calculate the origin to be in the center of the sprite (needed for rotation around the center), and adjust the position accordingly
	const Vector2 origin
	{
		m_SrcRect.width * s_GlobalScale.x * .5f,
		m_SrcRect.height * s_GlobalScale.y * .5f
	};

	DrawTexturePro
	(
		*Game::GetSpriteSheet(),
		m_SrcRect,
		{
			m_Position.x + origin.x,
			m_Position.y + origin.y,
			m_SrcRect.width * (m_Scale.x * s_GlobalScale.x),
			m_SrcRect.height * (m_Scale.y * s_GlobalScale.y)
		},
		origin,
		m_Rotation,
		WHITE
	);
	const Vector2 pos{ m_Position.x + ((m_Position.x * s_GlobalScale.x) - m_Position.x), m_Position.y + ((m_Position.y * s_GlobalScale.y) - m_Position.y) };
	DrawCircleV(pos, 4, RED);
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
	// Centers the sprite on the screen (Unscaled)
	m_Position.x = static_cast<float>(GetScreenWidth()) * .5f - (m_SrcRect.width * s_GlobalScale.x) * .5f;
	m_Position.y = static_cast<float>(GetScreenHeight()) * .5f - (m_SrcRect.height * s_GlobalScale.y) * .5f;
}

void Sprite::ScalePosition(const Vector2& scale)
{
	// Scales the position of the sprite.
	// Used for keeping the same relative position when resizing the window.
	m_Position.x *= scale.x;
	m_Position.y *= scale.y;
}

void Sprite::Rotate(const float degrees)
{
	m_Rotation += degrees;
}
