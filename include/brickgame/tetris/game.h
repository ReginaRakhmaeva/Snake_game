// game.h - API для Tetris (userInput/updateCurrentState)
#ifndef BRICKGAME_TETRIS_GAME_H_
#define BRICKGAME_TETRIS_GAME_H_

#include "../../../include/brickgame/common/types.h"

// Обработка пользовательского ввода
void userInput(UserAction_t action, bool hold);

// Обновление состояния игры и возврат актуальной информации
GameInfo_t updateCurrentState(void);
bool isGameOver(void);
#endif  // BRICKGAME_TETRIS_GAME_H_