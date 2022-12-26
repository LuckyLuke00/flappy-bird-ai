#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

class Sprite;
struct Vector2;

class Animation final
{
public:
	explicit Animation(Sprite& sprite, const std::vector<Vector2>& frameCoords, const int animFrames, const float frameDuration, const bool boomerang = false);

	Animation(const Animation& other) = delete;
	Animation& operator=(const Animation& other) = delete;
	Animation(Animation&& other) = delete;
	Animation& operator=(Animation&& other) = delete;

	~Animation() = default;

	void Update(float elapsedSec);

	void Play();
	void Pause();
	void Stop();

	// Setters
	void SetFrameDuration(const float duration) { m_FrameDuration = duration; }
	void SetFrameStart(const int frame);

private:
	std::vector<Vector2> m_FrameCoords;

	bool m_IsPlaying{ false };
	bool m_Boomerang{ false };
	bool m_Reverse{ false };

	float m_FrameDuration;
	float m_FrameTimer{ 0.f };

	int m_CurrentFrame{ 0 };
	int m_AnimFrames{ 0 };
	int m_FrameStart{ 0 };

	Sprite& m_Sprite;

	bool CheckBounds();
	void AdvanceFrames(const int numFramesToAdvance);
};

#endif
