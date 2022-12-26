#include "bird.h"
#include <iostream>

Bird::Bird()
	: m_BirdAnimation{ m_BirdSprite, m_BirdAnimationFrames, m_AnimFrames, m_FrameDuration, m_Boomerang }
{
	SelectRandomBird();
	m_BirdAnimation.Play();
}

void Bird::Update(float elapsedSec)
{
	m_BirdAnimation.Update(elapsedSec);
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

void Bird::Draw() const
{
	m_BirdSprite.Draw();
}
