#ifndef PIPE_H
#define PIPE_H

#include "sprite.h"

class Pipe final
{
public:
	explicit Pipe() = default;

	Pipe(const Pipe& other) = delete;
	Pipe& operator=(const Pipe& other) = delete;
	Pipe(Pipe&& other) = delete;
	Pipe& operator=(Pipe&& other) = delete;

	~Pipe() = default;

	void Draw() const;
	void Update(float elapsedSec);

	// Getters
	const Vector2& GetPosition() const { return m_BottomPipeSprite.GetPosition(); }
	float GetWidth() const { return m_BottomPipeSprite.GetWidth(); }
	const Rectangle& GetHitBoxBottom() const { return m_HitBoxBottom; }
	const Rectangle& GetHitBoxTop() const { return m_HitBoxTop; }

	// Setters
	void SetPosX(const float x) { m_BottomPipeSprite.SetPosX(x); m_TopPipeSprite.SetPosX(x); UpdatePosPercent(); }
	void SetPosY(const float y) { m_BottomPipeSprite.SetPosY(y); m_TopPipeSprite.SetPosY(y); UpdatePosPercent(); }
	void AddPosX(const float x) { m_BottomPipeSprite.AddPosX(x); m_TopPipeSprite.AddPosX(x); UpdatePosPercent(); }
	void AddPosY(const float y) { m_BottomPipeSprite.AddPosY(y); m_TopPipeSprite.AddPosY(y); UpdatePosPercent(); }

	void Initialize(const float offsetX);
	bool IsOffScreen() const;
	bool HasPassed(const Vector2& birdPos);
	void RefreshPosition();
private:
	Rectangle m_HitBoxTop{};
	Rectangle m_HitBoxBottom{};

	float m_PosPercentX{ .0f };
	float m_PosPercentYTop{ .0f };
	float m_PosPercentYBottom{ .0f };
	bool m_Passed{ false };

	Sprite m_TopPipeSprite{ { 56.f, 323.f, 26.f, 160.f } };
	Sprite m_BottomPipeSprite{ { 84.f, 323.f, 26.f, 160.f } };

	void SetRandomHeight();
	void UpdatePosPercent();

	// Function that sets the pipes to the right of the screen
	void SetToRightOfScreen();
	void AddVerticalGap();
	void UpdateHitBoxes();
};

#endif
