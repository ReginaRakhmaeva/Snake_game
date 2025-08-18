#ifndef GUI_CLI_INPUT_H
#define GUI_CLI_INPUT_H

#include <stdbool.h>

#include "../../../include/brickgame/common/types.h"
#include "app_controller.h"

UserAction_t read_input(bool *hold, GameType game_type);

#endif  // GUI_CLI_INPUT_H