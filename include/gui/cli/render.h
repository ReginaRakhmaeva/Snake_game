#ifndef BRICKGAME_TETRIS_RENDER_H_
#define BRICKGAME_TETRIS_RENDER_H_

#include "../../../include/brickgame/common/types.h"
#include "app_controller.h"

void render_game(const GameInfo_t *info);
void renderStartScreen();
void renderGameOverScreen();
void renderGameWonScreen();
GameType render_game_selection();
void render_loading_error(const char* error);

#endif  // BRICKGAME_TETRIS_RENDER_H_