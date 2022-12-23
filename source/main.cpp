#ifndef NDEBUG // Include Visual Leak Detector if we're in debug mode
#include <vld.h>
#endif

#include "constants.h"
#include "game.h"

int main()
{
	// Initialize the window dimensions
	constexpr int windowWidth{ 720 };
	constexpr int windowHeight{ 1280 };

	// Initialize the game window with the desired dimensions
	InitWindow(windowWidth, windowHeight, "Flappy Bird AI");

	// Set the icon for the game window
	const Image* pIcon{ new Image{ LoadImage(GAME_ICON_PATH) } };
	SetWindowIcon(*pIcon);

	Game game{};

	// Main game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	// Unload the icon
	UnloadImage(*pIcon);
	delete pIcon;
	pIcon = nullptr;

	return 0;
};
