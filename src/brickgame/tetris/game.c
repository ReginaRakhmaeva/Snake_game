#include "../../../include/brickgame/tetris/game.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/brickgame/common/types.h"
#include "../../../include/brickgame/tetris/backend.h"
#include "../../../include/brickgame/tetris/fsm.h"

static GameInfo_t game_info;


static void reset_game_info(void) {
  backend_free_game_info(&game_info);
  
  // Инициализируем backend
  backend_init_game();
  
  // Копируем инициализированную структуру из backend
  GameInfo_t backend_info = backend_get_info();
  
  // Копируем скалярные значения
  game_info.score = backend_info.score;
  game_info.high_score = backend_info.high_score;
  game_info.level = backend_info.level;
  game_info.speed = backend_info.speed;
  game_info.pause = backend_info.pause;
  
  // Копируем указатели на массивы
  game_info.field = backend_info.field;
  game_info.next = backend_info.next;
}

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

EXPORT GameInfo_t updateCurrentState() {
  GameState_t state = fsm_get_state();
  
  // Инициализируем game_info если она не инициализирована
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

  // Обновляем только скалярные значения
  GameInfo_t backend_info = backend_get_info();
  game_info.score = backend_info.score;
  game_info.high_score = backend_info.high_score;
  game_info.level = backend_info.level;
  game_info.speed = backend_info.speed;
  game_info.pause = (state == STATE_PAUSED);

  // Обновляем поле с текущей фигурой
  backend_overlay_piece(&game_info);

  return game_info;
}

EXPORT bool isGameOver(void) { 
  return fsm_get_state() == STATE_GAME_OVER; 
}
