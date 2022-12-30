#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr float FLAP_FORCE{ 140.f * 2.f };
constexpr float GRAVITY{ -9.81f * 2.f };
constexpr float MAX_FALL_SPEED{ 200.f * 2.f };

constexpr int GAME_WIDTH{ 144 };
constexpr int GAME_HEIGHT{ 256 };

const char* const GAME_ICON_PATH{ "../assets/images/icon.png" };
const char* const SPRITE_SHEET_PATH{ "../assets/images/spritesheet.png" };
const char* const WINDOW_TITLE{ "Flappy Bird AI" };

#endif
