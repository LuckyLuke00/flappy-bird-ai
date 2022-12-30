#ifndef CONSTANTS_H
#define CONSTANTS_H

// Bird constants
constexpr float FLAP_FORCE{ 600.f };
constexpr float GRAVITY{ 2000.f };
constexpr float MAX_FALL_SPEED{ 2500.f };
constexpr float ROTATION_SPEED_UP{ 600.f };
constexpr float ROTATION_SPEED_DOWN{ 480.f };
constexpr float MAX_UPWARD_ROTATION{ 20.f };
constexpr float MAX_DOWNWARD_ROTATION{ 90.f };

constexpr int GAME_WIDTH{ 144 };
constexpr int GAME_HEIGHT{ 256 };

const char* const GAME_ICON_PATH{ "../assets/images/icon.png" };
const char* const SPRITE_SHEET_PATH{ "../assets/images/spritesheet.png" };
const char* const WINDOW_TITLE{ "Flappy Bird AI" };

#endif
