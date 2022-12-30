#ifndef BIRD_H
#define BIRD_H

#include "sprite.h"
#include "animation.h"

class Bird final
{
public:
	explicit Bird();

	Bird(const Bird& other) = delete;
	Bird& operator=(const Bird& other) = delete;
	Bird(Bird&& other) = delete;
	Bird& operator=(Bird&& other) = delete;

	~Bird() = default;

	void Draw() const;
	void Update(float elapsedSec);
	void UpdateAnimation(float elapsedSec);

	void Flap();

	// Setters
	void SelectRandomBird();
	void Reset();

	// Getters
	const Vector2& GetPosition() const { return m_BirdSprite.GetPosition(); }
	const float GetHeight() const { return m_BirdSprite.GetHeight(); }

private:
	float m_VerticalSpeed{ .0f };
	float m_FlapStartPos{ .0f };
	Sprite m_BirdSprite{ { 3.f, 491.f, 17.f, 12.f } };

	// Animation
	const std::vector<Vector2> m_BirdAnimationFrames
	{
		{ 3.f, 491.f }, { 31.f, 491.f }, { 59.f, 491.f },		// Yellow bird
		{ 115.f, 381.f }, { 115.f, 407.f }, { 115.f, 433.f },	// Red bird
		{ 87.f, 491.f }, { 115.f, 329.f }, { 115.f, 355.f },	// Blue bird
	};

	const int m_AnimFrames{ 3 };
	const bool m_Boomerang{ true };
	const float m_FrameDuration{ .125f };

	Animation m_BirdAnimation;

	// Function that checks if the bird is out of the top of the screen
	bool IsOutOfBounds() const;
	bool IsFalling() const;

	// Function that rotates the bird sprite
	void RotateBird(float elapsedSec);
};

#endif
