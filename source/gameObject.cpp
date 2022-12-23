#include "gameObject.h"

void GameObject::SetPosition(const Vector2& position)
{
	m_Position = position;
}

void GameObject::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

Vector2 GameObject::GetPosition() const
{
	return m_Position;
}
