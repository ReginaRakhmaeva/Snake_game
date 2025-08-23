#include <stdbool.h>
// types.h - Общие типы для brickgame
#ifndef BRICKGAME_COMMON_TYPES_H
#define BRICKGAME_COMMON_TYPES_H

// Макрос для экспорта функций в разделяемых библиотеках
#define EXPORT __attribute__((visibility("default")))

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
#ifdef __cplusplus
enum class CellType { Empty = 0, Snake = 1, Apple = 2 };
#else
typedef enum { Empty = 0, Snake = 1, Apple = 2 } CellType;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

#ifdef __cplusplus
}
#endif

#endif  // BRICKGAME_COMMON_TYPES_H