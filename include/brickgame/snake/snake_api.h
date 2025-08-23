// snake_api.h - API для Snake (userInput/updateCurrentState)
#ifndef S21_SNAKE_API_H
#define S21_SNAKE_API_H

#include <stdbool.h>

#include "../common/types.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT bool isVictory(void);
EXPORT void userInput(UserAction_t action, bool hold);
EXPORT GameInfo_t updateCurrentState();
EXPORT bool isGameOver();

#ifdef __cplusplus
}
#endif

#endif  // S21_SNAKE_API_H