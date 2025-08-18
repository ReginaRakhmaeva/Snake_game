// snake_api.h - API для Snake (userInput/updateCurrentState)
#ifndef S21_SNAKE_API_H
#define S21_SNAKE_API_H

#include <stdbool.h>

#include "../common/types.h"

#ifdef __cplusplus
extern "C" {
#endif
bool isVictory(void);
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
bool isGameOver();

#ifdef __cplusplus
}
#endif

#endif  // S21_SNAKE_API_H