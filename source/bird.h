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

	// Function that rotates the bird sprite
	void CalculateBirdPipeHeightDelta(const Vector2& pipeCenter);
	void Flap();
	void RotateBird(float elapsedSec);
	void MutateJumpAtDelta();

	// Setters
	void SelectRandomBird();
	void Initialize();
	void RefreshPosition();
	void SetDeath(const bool isDead) { m_Dead = isDead; }
	void SetJumpAtDelta(const float jumpAtDelta) { m_JumpAtDelta = jumpAtDelta; }

	// Getters
	bool IsDead() const { return m_Dead; }
	const Vector2& GetPosition() const { return m_BirdSprite.GetPosition(); }
	float GetFitness() const { return m_Fitness; }
	float GetHitCircleRadius() const { return m_Radius * Sprite::GetGlobalScale().y; }
	float GetJumpAtDelta() const { return m_JumpAtDelta; }
	float GetScaledHeight() const { return m_BirdSprite.GetScaledHeight(); }
	Vector2 GetHitCircleCenter() const { return m_BirdSprite.GetCenter(); }

private:
	// Genetic Algorithm member variables
	float m_Fitness{ .0f };
	float m_JumpAtDelta{ .0f };

	bool m_Dead{ false };
	const Vector2 m_Offset{ -28.f, -1.f };
	float m_BirdPipeDelta{ .0f };
	float m_FlapStartPos{ .0f };
	float m_Radius{ 5.f };
	float m_VerticalSpeed{ .0f };
	Sprite m_BirdSprite{ { 3.f, 491.f, 17.f, 12.f } };
	Vector2 m_PosPercent{ .0f, .0f };

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
	bool IsOnGround() const;
	bool IsFalling() const;

	// Function that selects the best birds from the population
};

#endif
