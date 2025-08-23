// game.h - API для Tetris (userInput/updateCurrentState)
#ifndef S21_TETRIS_GAME_H
#define S21_TETRIS_GAME_H

#include <stdbool.h>
#include "../common/types.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void userInput(UserAction_t action, bool hold);
EXPORT GameInfo_t updateCurrentState();
EXPORT bool isGameOver();

#ifdef __cplusplus
}
#endif

#endif // S21_TETRIS_GAME_H