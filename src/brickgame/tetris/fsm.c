#include "../../../include/brickgame/tetris/fsm.h"

#include "../../../include/brickgame/common/types.h"  // Для UserAction_t

static GameState_t current_state = STATE_INIT;

GameState_t fsm_get_state(void) { return current_state; }

void fsm_process_input(UserAction_t action) {
  switch (current_state) {
    case STATE_INIT:
      if (action == Start) {
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
        current_state = STATE_INIT;
      }
      break;
  }
}

void fsm_set_state(GameState_t state) { current_state = state; }
