#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <raylib.h>

class GameObject
{
public:
	GameObject() = default;

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	virtual ~GameObject() = default;

	virtual void Update(const float deltaTime) = 0;
	virtual void Draw() const = 0;

	void SetPosition(const Vector2& position);
	void SetPosition(float x, float y);
	Vector2 GetPosition() const;

private:
	Vector2 m_Position;
};

#endif
