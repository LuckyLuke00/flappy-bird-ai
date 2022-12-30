#include "bird.h"
#include "constants.h"

Bird::Bird()
	: m_BirdAnimation{ m_BirdSprite, m_BirdAnimationFrames, m_AnimFrames, m_FrameDuration, m_Boomerang }
{
	SelectRandomBird();
	m_BirdAnimation.Play();
}

void Bird::Update(float elapsedSec)
{
	// Apply gravity
	m_VerticalSpeed += GRAVITY * elapsedSec;

	// Clamp the vertical fall speed
	if (m_VerticalSpeed > MAX_FALL_SPEED)
	{
		m_VerticalSpeed = MAX_FALL_SPEED;
	}

	// Update the position
	m_BirdSprite.SetPosY(m_BirdSprite.GetPosition().y + m_VerticalSpeed * elapsedSec);

	RotateBird(elapsedSec);
}

void Bird::Draw() const
{
	m_BirdSprite.Draw();
}

void Bird::UpdateAnimation(float elapsedSec)
{
	m_BirdAnimation.Update(elapsedSec);
}

void Bird::Flap()
{
	if (IsOutOfBounds()) return;

	m_FlapStartPos = m_BirdSprite.GetPosition().y;
	m_VerticalSpeed = -FLAP_FORCE;
}

void Bird::SelectRandomBird()
{
	m_BirdAnimation.SetFrameStart
	(
		GetRandomValue(0, static_cast<int>(m_BirdAnimationFrames.size() / m_AnimFrames)) * m_AnimFrames
	);
}

void Bird::Reset()
{
	m_BirdSprite.CenterOnScreen();
	m_BirdAnimation.Stop();
	m_BirdAnimation.Play();
}

bool Bird::IsOutOfBounds() const
{
	// Check if the bird is out of the top of the screen
	return m_BirdSprite.GetPosition().y < .0f;
}

bool Bird::IsFalling() const
{
	return (m_BirdSprite.GetPosition().y > m_FlapStartPos);
}

void Bird::RotateBird(float elapsedSec)
{
	if (m_VerticalSpeed < 0)
	{
		m_BirdSprite.Rotate(-ROTATION_SPEED_UP * elapsedSec);

		if (m_BirdSprite.GetRotation() < -MAX_UPWARD_ROTATION)
		{
			m_BirdSprite.SetRotation(-MAX_UPWARD_ROTATION);
		}
	}
	else if (IsFalling())
	{
		m_BirdSprite.Rotate(ROTATION_SPEED_DOWN * elapsedSec);
		if (m_BirdSprite.GetRotation() > MAX_DOWNWARD_ROTATION)
		{
			m_BirdSprite.SetRotation(MAX_DOWNWARD_ROTATION);
		}
	}
}
