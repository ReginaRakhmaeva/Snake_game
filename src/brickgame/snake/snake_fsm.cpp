#include "../../../include/brickgame/snake/snake_fsm.hpp"

namespace s21 {

SnakeFSM::SnakeFSM(SnakeGame& game) : game_(game) {}

void SnakeFSM::HandleInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      if (game_.GetState() == SnakeGameState::Ready ||
          game_.GetState() == SnakeGameState::Lost ||
          game_.GetState() == SnakeGameState::Won) {
        game_.Reset();
        game_.Resume();
      }
      break;
    case Pause:
      if (game_.GetState() == SnakeGameState::Running) {
        game_.Pause();
      } else if (game_.GetState() == SnakeGameState::Paused) {
        game_.Resume();
      }
      break;
    case Terminate:
      game_.Terminate();
      break;
    case Up:
      game_.ChangeDirection(Up);
      break;
    case Down:
      game_.ChangeDirection(Down);
      break;
    case Left:
      game_.ChangeDirection(Left);
      break;
    case Right:
      game_.ChangeDirection(Right);
      break;
    case Action:
      game_.Accelerate(hold);
      break;
    default:
      break;
  }
}

}  // namespace s21
