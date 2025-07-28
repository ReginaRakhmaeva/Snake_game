#ifndef GUI_CLI_INPUT_H
#define GUI_CLI_INPUT_H

#include <stdbool.h>

#include "../../../include/brickgame/common/types.h"

typedef enum {
    GAME_TETRIS = 1,
    GAME_SNAKE = 2
} GameType;

UserAction_t read_input(bool *hold, GameType game_type);

#endif  // GUI_CLI_INPUT_H