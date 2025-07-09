// fsm.h - Конечный автомат для Tetris
#ifndef BRICKGAME_TETRIS_FSM_H_
#define BRICKGAME_TETRIS_FSM_H_

#include "../../../include/brickgame/common/types.h"  // Для UserAction_t

typedef enum {
  STATE_INIT,
  STATE_RUNNING,
  STATE_PAUSED,
  STATE_GAME_OVER
} GameState_t;

// Получить текущее состояние
GameState_t fsm_get_state(void);

// Обновить состояние автомата на основе действия пользователя
void fsm_process_input(UserAction_t action);

// Принудительно установить состояние
void fsm_set_state(GameState_t state);

#endif  // BRICKGAME_TETRIS_FSM_H_ 