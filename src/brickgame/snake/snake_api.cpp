#include "../../../include/brickgame/snake/snake_api.h"

#include "../../../include/brickgame/snake/snake_fsm.hpp"
#include "../../../include/brickgame/snake/snake_game.hpp"

namespace s21 {
static SnakeGame game;
static SnakeFSM fsm(game);
}  // namespace s21

extern "C" void userInput(UserAction_t action, bool hold) {
  s21::fsm.HandleInput(action, hold);
}

extern "C" GameInfo_t updateCurrentState() {
  // Двигаем змейку на 1 шаг, если игра в состоянии Running
  if (s21::game.GetState() == s21::SnakeGameState::Running) {
    s21::game.Update();
  }
  return s21::game.GetGameInfo();
}

extern "C" bool isGameOver() {
  auto state = s21::game.GetState();
  // Возвращаем true только для явных состояний завершения
  return state == s21::SnakeGameState::Lost ||
         state == s21::SnakeGameState::Won ||
         state == s21::SnakeGameState::Ready;  // Добавили Ready
}
