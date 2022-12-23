#ifndef STRUCTS_H
#define STRUCTS_H

#include <raylib.h>

struct Sprite
{
	Rectangle sourceRect;
	Vector2 position{ .0f, .0f };
	float rotation{ .0f };
	float scale{ 1.f };
	Color tint{ WHITE };

	void Draw(const Texture2D* pTexture) const
	{
		DrawTexturePro(*pTexture, sourceRect, { position.x, position.y, sourceRect.width * scale, sourceRect.height * scale }, {}, rotation, tint);
	}

	void ScaleToFitScreen()
	{
		const float screenWidth{ static_cast<float>(GetScreenWidth()) };
		const float screenHeight{ static_cast<float>(GetScreenHeight()) };

		const float screenRatio{ screenWidth / screenHeight };
		const float spriteRatio{ sourceRect.width / sourceRect.height };

		// We need to add one
		if (screenRatio > spriteRatio)
		{
			scale = screenHeight / sourceRect.height;
		}
		else
		{
			scale = screenWidth / sourceRect.width;
		}
	}
};
#endif
