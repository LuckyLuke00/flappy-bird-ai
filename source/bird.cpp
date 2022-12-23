#include "bird.h"
#include "constants.h"

Bird::Bird(const Vector2& position)
	: GameObject()
{
	SetPosition(position);
}

Bird::~Bird()
{
	UnloadTexture(*m_pTexture);

	delete m_pTexture;
	m_pTexture = nullptr;
}

void Bird::Update()
{
	m_Velocity.y += GRAVITY;

	SetPosition(GetPosition().x, GetPosition().y + m_Velocity.y);
}

void Bird::Draw()
{
}
