#ifndef CONSTANTS_H
#define CONSTANTS_H

// Bird constants
constexpr float FLAP_FORCE{ 150.f };
constexpr float GRAVITY{ 500.f };
constexpr float MAX_FALL_SPEED{ 250.f };
constexpr float ROTATION_SPEED_UP{ 600.f };
constexpr float ROTATION_SPEED_DOWN{ 480.f };
constexpr float MAX_UPWARD_ROTATION{ 20.f };
constexpr float MAX_DOWNWARD_ROTATION{ 90.f };

// Game constants
constexpr float MOVE_SPEED{ 60.f }; // 60
constexpr int GAME_WIDTH{ 144 };
constexpr int GAME_HEIGHT{ 256 };

// Pipe constants
constexpr int PIPE_VERTICAL_GAP{ 50 };
constexpr int PIPE_HORIZONTAL_GAP{ 55 };
constexpr int MAX_PIPES{ 3 };
constexpr int MAX_PIPE_HEIGHT{ 100 };

// Path constants
const char* const GAME_ICON_PATH{ "../assets/images/icon.png" };
const char* const SPRITE_SHEET_PATH{ "../assets/images/spritesheet.png" };

// Window constants
const char* const WINDOW_TITLE{ "Flappy Bird AI" };

#endif
