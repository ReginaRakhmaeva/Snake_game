
/**
 * @file fsm.c
 * @brief Реализация конечного автомата (FSM) для управления состояниями игры
 * Tetris.
 *
 * FSM отвечает за переходы между состояниями игры:
 * - STATE_INIT — начальное состояние перед запуском;
 * - STATE_RUNNING — активная игра;
 * - STATE_PAUSED — игра на паузе;
 * - STATE_GAME_OVER — завершённая игра.
 *
 * Управление состояниями выполняется на основе пользовательского ввода
 * (Start, Pause, Terminate) через функцию fsm_process_input().
 */
#include "../../include/brickgame/tetris/fsm.h"

#include "../../include/brickgame/common/types.h"

static GameState_t current_state = STATE_INIT;
/**
 * @brief Возвращает текущее состояние FSM.
 *
 * @return GameState_t текущее состояние игры.
 */
GameState_t fsm_get_state(void) { return current_state; }
/**
 * @brief Обрабатывает пользовательский ввод и изменяет состояние FSM.
 *
 * @param action действие пользователя (Start, Pause, Terminate).
 */
void fsm_process_input(UserAction_t action) {
  switch (current_state) {
    case STATE_INIT:
      if (action == Start && current_state == STATE_INIT) {
        current_state = STATE_RUNNING;
      }
      break;

    case STATE_RUNNING:
      if (action == Pause) {
        current_state = STATE_PAUSED;
      } else if (action == Terminate) {
        current_state = STATE_GAME_OVER;
      }
      break;

    case STATE_PAUSED:
      if (action == Pause) {
        current_state = STATE_RUNNING;
      } else if (action == Terminate) {
        current_state = STATE_GAME_OVER;
      }
      break;

    case STATE_GAME_OVER:
      if (action == Start) {
        current_state = STATE_RUNNING;
      }
      break;
  }
}
/**
 * @brief Принудительно устанавливает состояние FSM.
 *
 * @param state новое состояние.
 */
void fsm_set_state(GameState_t state) { current_state = state; }
