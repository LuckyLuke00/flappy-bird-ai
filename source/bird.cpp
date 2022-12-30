#include "bird.h"
#include <iostream>
#include "constants.h"

Bird::Bird()
	: m_BirdAnimation{ m_BirdSprite, m_BirdAnimationFrames, m_AnimFrames, m_FrameDuration, m_Boomerang }
{
	SelectRandomBird();
	m_BirdAnimation.Play();
}

void Bird::Update()
{
	m_BirdAnimation.Update(GetFrameTime());

	// TODO: Seperate the below into functions
	MoveBird();
}

void Bird::Flap()
{
	m_VerticalSpeed = -500;
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

void Bird::MoveBird()
{
	m_BirdSprite.SetPosition({ m_BirdSprite.GetPosition().x, m_BirdSprite.GetPosition().y + m_VerticalSpeed * GetFrameTime() });
	m_VerticalSpeed += 1000 * GetFrameTime();
}

void Bird::Draw() const
{
	m_BirdSprite.Draw();
}
