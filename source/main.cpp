#ifndef NDEBUG // Include Visual Leak Detector if we're in debug mode
#include <vld.h>
#endif

#include "constants.h"
#include "game.h"

int main(void)
{
	// Window properties
	const Image icon{ LoadImage(GAME_ICON_PATH) };

	// Should always be 9:16
	constexpr int screenHeight{ 1280 };
	constexpr int screenWidth{ 720 };

	// Window initialization
	InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
	SetWindowIcon(icon);

	// This needs to be after the window initialization, Textures require a valid OpenGL context.
	Game* pGame{ new Game{} };
	pGame->ToggleFps();

	// Main game loop
	while (!WindowShouldClose())
	{
		pGame->Update(GetFrameTime());

		pGame->Draw();
	}

	// Clean-up resources
	UnloadImage(icon);

	delete pGame;
	pGame = nullptr;

	CloseWindow();

	return 0;
}
