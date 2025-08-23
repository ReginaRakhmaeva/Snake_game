#include "../../../include/brickgame/snake/snake_api.h"

#include "../../../include/brickgame/snake/snake_fsm.hpp"
#include "../../../include/brickgame/snake/snake_game.hpp"

namespace s21 {
static SnakeGame game;
static SnakeFSM fsm(game);

}  // namespace s21
static GameInfo_t previous_info;

extern "C" EXPORT void userInput(UserAction_t action, bool hold) {
  s21::fsm.HandleInput(action, hold);
}

extern "C" EXPORT GameInfo_t updateCurrentState() {
  s21::game.Tick();
  s21::game.FreeGameInfo(previous_info);
  previous_info = s21::game.GetGameInfo();
  return previous_info;
}

extern "C" EXPORT bool isGameOver() {
  auto state = s21::game.GetState();
  return state == s21::SnakeGameState::Lost ||
         state == s21::SnakeGameState::Won;
}

extern "C" EXPORT bool isVictory() {
  return s21::game.GetState() == s21::SnakeGameState::Won;
}
