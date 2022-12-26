#include "animation.h"
#include "sprite.h"

#include <iostream>

Animation::Animation(Sprite& sprite, const std::vector<Vector2>& frameCoords, const int animFrames, const float frameDuration, const bool boomerang)
	: m_FrameCoords{ frameCoords },
	m_Boomerang{ boomerang },
	m_FrameDuration{ frameDuration },
	m_AnimFrames{ animFrames },
	m_Sprite{ sprite }
{
	CheckBounds();
}

void Animation::Update(float elapsedSec)
{
	// Return if animation is not playing or frame timer is already greater than frame duration.
	if (!m_IsPlaying || m_FrameTimer >= m_FrameDuration) return;

	// Update frame timer.
	m_FrameTimer += elapsedSec;

	// Calculate number of frames to advance based on elapsed time and frame duration.
	AdvanceFrames(static_cast<int>(m_FrameTimer / m_FrameDuration));

	// Update source rect position of sprite.
	m_Sprite.SetSrcRectPos(m_FrameCoords[m_CurrentFrame]);
}

void Animation::Play()
{
	m_IsPlaying = true;
}

void Animation::Pause()
{
	m_IsPlaying = false;
}

void Animation::Stop()
{
	m_IsPlaying = false;

	m_CurrentFrame = m_FrameStart;
	m_FrameTimer = .0f;
}

bool Animation::CheckBounds()
{
	// Check if the frame start is within bounds.
	if (m_FrameStart < 0 || m_FrameStart >= static_cast<int>(m_FrameCoords.size()))
	{
		std::cerr << "WARNING: Frame start is out of bounds.\n";

		m_FrameStart = 0;

		return false;
	}

	// Check if the number of frames is within bounds.
	if (m_AnimFrames < 0 || m_FrameStart + m_AnimFrames > static_cast<int>(m_FrameCoords.size()))
	{
		std::cerr << "WARNING: Number of frames is out of bounds.\n";

		m_FrameStart = 0;
		m_AnimFrames = static_cast<int>(m_FrameCoords.size());

		return false;
	}

	return true;
}

void Animation::AdvanceFrames(const int numFramesToAdvance)
{
	// Subtract number of frames advanced from frame timer.
	m_FrameTimer -= static_cast<float>(numFramesToAdvance) * m_FrameDuration;

	if (!m_Boomerang)
	{
		// Advance current frame and set it to m_FrameStart if it reaches m_FrameStart + m_AnimFrames.
		m_CurrentFrame = (m_CurrentFrame + numFramesToAdvance) % (m_FrameStart + m_AnimFrames);
		if (m_CurrentFrame == 0)
		{
			m_CurrentFrame = m_FrameStart;
		}
	}
	else if (m_Reverse)
	{
		// Decrease current frame and set m_Reverse to false if current frame becomes m_FrameStart.
		m_CurrentFrame = std::max(m_FrameStart, m_CurrentFrame - numFramesToAdvance);
		if (m_CurrentFrame == m_FrameStart)
		{
			m_Reverse = false;
		}
	}
	else
	{
		// Advance current frame and set m_Reverse to true if current frame becomes m_FrameStart + m_AnimFrames.
		m_CurrentFrame = (m_CurrentFrame + numFramesToAdvance) % (m_FrameStart + m_AnimFrames);
		if (m_CurrentFrame == m_FrameStart + m_AnimFrames - 1)
		{
			m_Reverse = true;
		}
	}
}

void Animation::SetFrameStart(const int frame)
{
	m_FrameStart = frame;
	m_CurrentFrame += m_FrameStart;

	CheckBounds();
}
