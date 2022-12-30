#ifndef NDEBUG // Include Visual Leak Detector if we're in debug mode
#include <vld.h>
#endif

#include "constants.h"
#include "game.h"

int main(void)
{
	// Window properties
	const Image icon{ LoadImage(GAME_ICON_PATH) };

	// Window initialization
	// Start minimized to prevent the window from flashing on the screen
	InitWindow(GAME_WIDTH, GAME_HEIGHT, WINDOW_TITLE);

	// Scale the window resolution according to the main monitor's resolution
	// We need to do this now because we can only get the monitor's resolution after the window is initialized

	// Get the active monitor
	const int monitor{ GetCurrentMonitor() };
	// Get the monitor's resolution
	const int monitorWidth{ GetMonitorWidth(monitor) };
	const int monitorHeight{ GetMonitorHeight(monitor) };

	// On a 4K screen the window should be about 720p
	const int windowWidth{ monitorWidth / 2 };
	const int windowHeight{ monitorHeight / 2 };

	// Set the window's resolution
	SetWindowSize(windowWidth, windowHeight);

	// Center the window
	SetWindowPosition((monitorWidth - windowWidth) / 2, (monitorHeight - windowHeight) / 2);

	// Set window to resizable
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowIcon(icon);

	// This needs to be after the window initialization, Textures require a valid OpenGL context.
	Game* pGame{ new Game{} };
	pGame->ToggleFps();

	// Main game loop
	while (!WindowShouldClose())
	{
		// Check if the window has been resized
		if (IsWindowResized())
		{
			pGame->OnWindowResize();
		}

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
