#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"

class Sprite
{
public:
	explicit Sprite(const Rectangle& srcRect, const Vector2& pos = { .0f, .0f }, const Vector2& scale = { 1.f, 1.f }, const float rotation = .0f);

	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(Sprite&& other) = delete;

	virtual ~Sprite() = default;

	void Draw() const;

	void FitScreenHeight();
	void FitScreenWidth();

	void CenterOnScreen();

	void Rotate(const float degrees);

	// Getters
	const Rectangle& GetSrcRect() const { return m_SrcRect; }
	const Vector2& GetPosition() const { return m_Position; }
	float GetRotation() const { return m_Rotation; }
	float GetScaledHeight() const { return m_SrcRect.height * (m_Scale.y * s_GlobalScale.y); }
	float GetScaledWidth() const { return m_SrcRect.width * (m_Scale.x * s_GlobalScale.x); }
	float GetWidth() const { return m_SrcRect.width; }
	float GetHeight() const { return m_SrcRect.height; }
	Vector2 GetScale() const { return m_Scale; }

	static const Vector2& GetGlobalScale() { return s_GlobalScale; }

	// Setters
	void AddPosX(const float x) { m_Position.x += x * (s_GlobalScale.x); }
	void AddPosY(const float y) { m_Position.y += y * (s_GlobalScale.y); }

	void SetPosition(const Vector2& pos) { m_Position = pos; }
	void SetPosX(const float x) { m_Position.x = x; }
	void SetPosY(const float y) { m_Position.y = y; }
	void SetRotation(const float degrees) { m_Rotation = degrees; }
	void SetScale(const Vector2& scale) { m_Scale = scale; }
	void SetSrcRectPos(const float x, const float y) { m_SrcRect.x = x; m_SrcRect.y = y; }
	void SetSrcRectPos(const Vector2& pos) { m_SrcRect.x = pos.x; m_SrcRect.y = pos.y; }

	static void SetGlobalScale(const Vector2& scale) { s_GlobalScale = scale; }

private:
	float m_Rotation;

	Rectangle m_SrcRect;

	Vector2 m_Position;
	Vector2 m_Scale;

	static Vector2 s_GlobalScale;
};

#endif
