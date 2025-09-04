/**
 * @file fsm.h
 * @brief Конечный автомат (FSM) для управления состояниями игры Tetris.
 *
 * Модуль отвечает за переходы между основными состояниями игры:
 * - STATE_INIT — начальное состояние (ожидание старта);
 * - STATE_RUNNING — активная игра;
 * - STATE_PAUSED — игра на паузе;
 * - STATE_GAME_OVER — завершение игры.
 *
 * FSM обрабатывает пользовательский ввод (Start, Pause, Terminate)
 * и управляет логикой переходов между состояниями.
 */
#ifndef BRICKGAME_TETRIS_FSM_H_
#define BRICKGAME_TETRIS_FSM_H_

#include "../common/types.h"

/**
 * @brief Возможные состояния игры.
 */
typedef enum {
  STATE_INIT,
  STATE_RUNNING,
  STATE_PAUSED,
  STATE_GAME_OVER
} GameState_t;

/**
 * @brief Возвращает текущее состояние FSM.
 *
 * @return GameState_t текущее состояние автомата.
 */
GameState_t fsm_get_state(void);

/**
 * @brief Обновляет состояние FSM на основе пользовательского ввода.
 *
 * @param action действие игрока (Start, Pause, Terminate).
 */
void fsm_process_input(UserAction_t action);

/**
 * @brief Принудительно устанавливает состояние FSM.
 *
 * @param state новое состояние.
 */
void fsm_set_state(GameState_t state);

#endif  // BRICKGAME_TETRIS_FSM_H_