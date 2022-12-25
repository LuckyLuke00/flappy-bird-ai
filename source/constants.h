#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr float FLAP_FORCE{ .25f };
constexpr float GRAVITY{ .5f };
constexpr float MAX_FALL_SPEED{ .25f };

constexpr int WINDOW_WIDTH{ 144 };
constexpr int WINDOW_HEIGHT{ 256 };

const char* const GAME_ICON_PATH{ "../assets/images/icon.png" };
const char* const SPRITE_SHEET_PATH{ "../assets/images/spritesheet.png" };
const char* const WINDOW_TITLE{ "Flappy Bird AI" };

#endif
