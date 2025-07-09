// backend.h - Чистая игровая логика для Tetris
#ifndef BRICKGAME_TETRIS_BACKEND_H_
#define BRICKGAME_TETRIS_BACKEND_H_

#include <stdio.h>
#define SCORE_FILE "tetris_highscore.txt"
#include "../../../include/brickgame/common/types.h"

typedef enum {
    BACKEND_OK,
    BACKEND_GAME_OVER
} BackendStatus;

// Инициализация игры (первый запуск или рестарт)
GameInfo_t backend_init_game(void);

// Обработка действия игрока
BackendStatus backend_handle_input(UserAction_t action, bool hold);

// Обновление физики игры (например, падение вниз по таймеру)
BackendStatus backend_update_physics(GameInfo_t *info);

BackendStatus backend_fix_piece(void);
int get_level_speed(int level);
//////////////////
void backend_overlay_piece(GameInfo_t *info_ptr);
void save_high_score(int high_score);

GameInfo_t backend_get_info(void);

#endif  // BRICKGAME_TETRIS_BACKEND_H_