#include "sprite.h"
#include "game.h"

Vector2 Sprite::s_GlobalScale{ 1.f, 1.f };

Sprite::Sprite(const Rectangle& srcRect, const Vector2& pos, const float rotation)
	: m_Rotation{ rotation },
	m_SrcRect{ srcRect },
	m_Position{ pos },
	m_InitialSrcRect{ srcRect }
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
			m_SrcRect.width * s_GlobalScale.x,
			m_SrcRect.height * s_GlobalScale.y
		},
		origin,
		m_Rotation,
		WHITE
	);
}

void Sprite::CenterOnScreen()
{
	// Centers the sprite on the screen (Unscaled)
	m_Position.x = static_cast<float>(GetScreenWidth()) * .5f - (m_SrcRect.width * s_GlobalScale.x) * .5f;
	m_Position.y = static_cast<float>(GetScreenHeight()) * .5f - (m_SrcRect.height * s_GlobalScale.y) * .5f;
}

void Sprite::Rotate(const float degrees)
{
	m_Rotation += degrees;
}
