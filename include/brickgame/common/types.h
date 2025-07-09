#include <stdbool.h>
// types.h - Общие типы для brickgame
#ifndef BRICKGAME_COMMON_TYPES_H
#define BRICKGAME_COMMON_TYPES_H

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

#endif // BRICKGAME_COMMON_TYPES_H 