#ifndef BIRD_H
#define BIRD_H

#include "gameObject.h"

class Bird : public GameObject
{
public:
	explicit Bird(const Vector2& position = { .0f, .0f });
	~Bird() override;

	virtual void Update() override;
	virtual void Draw() override;
private:
	Vector2 m_Velocity{ .0f, .0f };
	Texture2D* m_pTexture{ new Texture2D{ LoadTexture("bird.png") } };
};

#endif
