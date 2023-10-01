#ifndef CONSTANTS_H
#define CONSTANTS_H

// ---------- Bird Constants ----------
constexpr float FLAP_FORCE{ 150.f };
constexpr float GRAVITY{ 500.f };
constexpr float MAX_FALL_SPEED{ 250.f };
constexpr float ROTATION_SPEED_UP{ 600.f };
constexpr float ROTATION_SPEED_DOWN{ 480.f };
constexpr float MAX_UPWARD_ROTATION{ 20.f };
constexpr float MAX_DOWNWARD_ROTATION{ 90.f };

// ---------- Game Constants ----------
constexpr float MOVE_SPEED{ 60.f };
constexpr int GAME_WIDTH{ 144 };
constexpr int GAME_HEIGHT{ 256 };

// ---------- Pipe Constants ----------
constexpr int PIPE_VERTICAL_GAP{ 50 };
constexpr int PIPE_HORIZONTAL_GAP{ 55 };
constexpr int MAX_PIPES{ 3 };
constexpr int MAX_PIPE_HEIGHT{ 100 };

// ---------- Path Constants ----------
constexpr const char* const GAME_ICON_PATH{ "./assets/images/icon.png" };
constexpr const char* const SPRITE_SHEET_PATH{ "./assets/images/spritesheet.png" };

// --------- Window Constants ---------
constexpr const char* const WINDOW_TITLE{ "Flappy Bird AI" };

// ----------- AI Constants -----------
constexpr int POPULATION_SIZE{ 5 };
constexpr int SAMPLE_SIZE{ 3 };
constexpr float MUTATION_RATE{ .1f };

#endif
