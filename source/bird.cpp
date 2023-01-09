#include "bird.h"
#include "constants.h"
#include "game.h"
#include "utils.h"

#include <iostream>

Bird::Bird()
	: m_BirdAnimation{ m_BirdSprite, m_BirdAnimationFrames, m_AnimFrames, m_FrameDuration, m_Boomerang }
{
	// Initialize with random value
	m_JumpAtDelta = utils::RandomFloat(.0f, GAME_HEIGHT * .5f);
}

void Bird::Update(float elapsedSec)
{
	m_PosPercent.x = m_BirdSprite.GetPosition().x / static_cast<float>(GetScreenWidth());
	m_PosPercent.y = m_BirdSprite.GetPosition().y / static_cast<float>(GetScreenHeight());

	if (!m_Dead)
	{
		// Update distance traveled
		m_Fitness += MOVE_SPEED * elapsedSec;
	}

	if (!Game::IsGameOver() && m_Dead)
	{
		m_BirdSprite.AddPosX(-MOVE_SPEED * elapsedSec);
	}

	if (IsOnGround())
	{
		m_Dead = true;
		RotateBird(elapsedSec);
		return;
	}

	// Let the bird play it self
	if (m_BirdPipeDelta > m_JumpAtDelta * Sprite::GetGlobalScale().x)
	{
		Flap();
	}

	// Apply gravity
	m_VerticalSpeed += GRAVITY * elapsedSec;

	// Clamp the vertical fall speed
	if (m_VerticalSpeed > MAX_FALL_SPEED)
	{
		m_VerticalSpeed = MAX_FALL_SPEED;
	}

	// Update the position
	m_BirdSprite.AddPosY(m_VerticalSpeed * elapsedSec);

	RotateBird(elapsedSec);
}

void Bird::Draw() const
{
	m_BirdSprite.Draw();

	//// DEBUG
	//const Vector2& pos{ m_BirdSprite.GetPosition() };
	//const Vector2& center{ m_BirdSprite.GetCenter() };
	//DrawLine
	//(
	//	static_cast<int>(center.x),
	//	static_cast<int>(pos.y),
	//	static_cast<int>(center.x),
	//	static_cast<int>(pos.y - m_BirdPipeDelta),
	//	RED
	//);
}

void Bird::UpdateAnimation(float elapsedSec)
{
	if (IsDead()) return;
	m_BirdAnimation.Update(elapsedSec);
}

void Bird::CalculateBirdPipeHeightDelta(const Vector2& pipeCenter)
{
	m_BirdPipeDelta = m_BirdSprite.GetPosition().y + m_BirdSprite.GetScaledHeight() - pipeCenter.y;
}

void Bird::Flap()
{
	if (m_Dead || IsOutOfBounds()) return;

	m_FlapStartPos = static_cast<float>(GetScreenHeight()) * m_PosPercent.y;
	m_VerticalSpeed = -FLAP_FORCE;
}

void Bird::SelectRandomBird()
{
	m_BirdAnimation.SetFrameStart
	(
		GetRandomValue(0, (static_cast<int>(m_BirdAnimationFrames.size() / m_AnimFrames)) - 1) * m_AnimFrames
	);
}

void Bird::Initialize()
{
	SelectRandomBird();

	m_Dead = false;

	m_BirdSprite.SetRotation(0.f);
	m_BirdSprite.CenterOnScreen();

	// Reset fitness
	m_Fitness = .0f;

	// Add an offset
	m_BirdSprite.AddPosX(m_Offset.x);
	m_BirdSprite.AddPosY(m_Offset.y);

	m_PosPercent.x = m_BirdSprite.GetPosition().x / static_cast<float>(GetScreenWidth());
	m_PosPercent.y = m_BirdSprite.GetPosition().y / static_cast<float>(GetScreenHeight());

	m_BirdAnimation.Stop();
	m_BirdAnimation.Play();
}

void Bird::RefreshPosition()
{
	// Update the position of the bird based on the new screen dimensions
	m_BirdSprite.SetPosition({ static_cast<float>(GetScreenWidth()) * m_PosPercent.x, static_cast<float>(GetScreenHeight()) * m_PosPercent.y });
}

bool Bird::IsOutOfBounds() const
{
	// Check if the bird is out of the top of the screen
	return static_cast<float>(GetScreenHeight()) * m_PosPercent.y < .0f;
}

bool Bird::IsOnGround() const
{
	return m_BirdSprite.GetPosition().y + m_BirdSprite.GetScaledHeight() > static_cast<float>(GetScreenHeight()) - Game::GetGroundHeight() * Sprite::GetGlobalScale().x;
}

bool Bird::IsFalling() const
{
	return (static_cast<float>(GetScreenHeight()) * m_PosPercent.y > m_FlapStartPos);
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

void Bird::MutateJumpAtDelta()
{
	m_JumpAtDelta *= utils::RandomFloat(1.f - MUTATION_RATE, 1.f + MUTATION_RATE);
}
