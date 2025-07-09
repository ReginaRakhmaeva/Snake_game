#ifndef BRICKGAME_TETRIS_RENDER_H_
#define BRICKGAME_TETRIS_RENDER_H_

#include "../../../include/brickgame/common/types.h"

void render_game(const GameInfo_t *info);
void renderStartScreen();
void renderGameOverScreen();
#endif  // BRICKGAME_TETRIS_RENDER_H_