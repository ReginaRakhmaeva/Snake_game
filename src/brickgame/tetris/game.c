/**
 * @file game.c
 * @brief Внешний API для игры Tetris (BrickGame).
 *
 * Здесь реализуются функции, которые видят фронтенды (CLI, GUI и т.д.),
 * но вся игровая логика хранится во внутреннем backend.
 *
 * Основные задачи:
 * - обработка пользовательского ввода через FSM;
 * - обновление состояния игры на каждом тике;
 * - синхронизация отображаемой информации (GameInfo_t) с backend;
 * - определение окончания игры.
 */

#include "../../../include/brickgame/tetris/game.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/brickgame/common/types.h"
#include "../../../include/brickgame/tetris/backend.h"
#include "../../../include/brickgame/tetris/fsm.h"
#include "../../../include/brickgame/tetris/game.h"

static GameInfo_t game_info;

/**
 * @brief Сбрасывает текущее состояние игры и инициализирует новое.
 */
static void reset_game_info(void) {
  backend_free_game_info(&game_info);

  backend_init_game();

  GameInfo_t backend_info = backend_get_info();

  game_info.score = backend_info.score;
  game_info.high_score = backend_info.high_score;
  game_info.level = backend_info.level;
  game_info.speed = backend_info.speed;
  game_info.pause = backend_info.pause;

  game_info.field = backend_info.field;
  game_info.next = backend_info.next;
}

/**
 * @brief Обрабатывает ввод игрока.
 *
 * @param action действие пользователя (влево, вправо, поворот и т.д.)
 * @param hold   признак удержания кнопки
 */
EXPORT void userInput(UserAction_t action, bool hold) {
  fsm_process_input(action);
  GameState_t state = fsm_get_state();

  if (state == STATE_RUNNING) {
    BackendStatus status = backend_handle_input(action, hold);
    if (status == BACKEND_GAME_OVER) {
      fsm_set_state(STATE_GAME_OVER);
    }
  }
}

/**
 * @brief Обновляет состояние игры.
 *
 * Синхронизирует frontend-структуру GameInfo_t с backend.
 * Запускает физику падения фигур, проверяет завершение игры.
 *
 * @return текущее состояние GameInfo_t для отрисовки.
 */
EXPORT GameInfo_t updateCurrentState() {
  GameState_t state = fsm_get_state();

  if (!game_info.field) {
    reset_game_info();
  }

  if (state == STATE_INIT) {
    reset_game_info();
  }
  if (state == STATE_RUNNING) {
    BackendStatus status = backend_update_physics(&game_info);
    if (status == BACKEND_GAME_OVER) {
      fsm_set_state(STATE_GAME_OVER);
    }
  }

  GameInfo_t backend_info = backend_get_info();
  game_info.score = backend_info.score;
  game_info.high_score = backend_info.high_score;
  game_info.level = backend_info.level;
  game_info.speed = backend_info.speed;
  game_info.pause = (state == STATE_PAUSED);

  backend_overlay_piece(&game_info);

  return game_info;
}
/**
 * @brief Проверяет, закончена ли игра.
 *
 * @return true если игра завершена, иначе false.
 */
EXPORT bool isGameOver(void) { return fsm_get_state() == STATE_GAME_OVER; }
