#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <stdbool.h>
#include "../../brickgame/common/types.h"

// Перечисление поддерживаемых игр
typedef enum {
    GAME_TETRIS,
    GAME_SNAKE
} GameType;

// Структура для работы с API игры
typedef struct {
    void* lib_handle;
    void (*userInput)(UserAction_t action, bool hold);
    GameInfo_t (*updateState)(void);
    bool (*isOver)(void);
    bool valid;
    char* error;
} GameAPI;

// Основные функции контроллера
void run_app(void);
GameAPI load_game_lib(GameType game);
void unload_game_lib(GameAPI api);

#endif  // APP_CONTROLLER_H