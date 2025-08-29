/**
 * @file snake_fsm.cpp
 * @brief Реализация конечного автомата (FSM) для игры Snake.
 *
 * Канонический FSM с внутренним состоянием и четко определенными переходами.
 * FSM управляет состоянием игры и выполняет соответствующие действия.
 */
#include "../../../include/brickgame/snake/snake_fsm.hpp"

namespace s21 {

/**
 * @brief Конструктор FSM для Snake.
 *
 * @param game ссылка на SnakeGame, которым управляет FSM.
 */
SnakeFSM::SnakeFSM(SnakeGame& game)
    : game_(game), current_state_(SnakeGameState::Ready) {
  OnEnterReady();
}

/**
 * @brief Обрабатывает пользовательский ввод и выполняет переходы состояний.
 *
 * Канонический FSM с четко определенными переходами:
 * - Ready → Running (Start)
 * - Running → Paused (Pause)
 * - Paused → Running (Pause)
 * - Running → Lost (Terminate)
 * - Lost → Ready (Start)
 * - Won → Ready (Start)
 *
 * @param action действие пользователя (Start, Pause, Terminate и т.д.)
 * @param hold признак удержания кнопки (актуально для ускорения).
 */
void SnakeFSM::HandleInput(UserAction_t action, bool hold) {
  switch (current_state_) {
    case SnakeGameState::Ready:
      if (action == Start) {
        TransitionTo(SnakeGameState::Running);
      }
      break;

    case SnakeGameState::Running:
      switch (action) {
        case Pause:
          TransitionTo(SnakeGameState::Paused);
          break;
        case Terminate:
          TransitionTo(SnakeGameState::Lost);
          break;
        case Up:
        case Down:
        case Left:
        case Right:
          game_.ChangeDirection(action, hold);
          break;
        case Action:
          game_.Accelerate(hold);
          break;
        default:
          break;
      }
      break;

    case SnakeGameState::Paused:
      if (action == Pause) {
        TransitionTo(SnakeGameState::Running);
      } else if (action == Terminate) {
        TransitionTo(SnakeGameState::Lost);
      }
      break;

    case SnakeGameState::Lost:
    case SnakeGameState::Won:
      if (action == Start) {
        TransitionTo(SnakeGameState::Ready);
      }
      break;
  }
}

void SnakeFSM::TransitionTo(SnakeGameState new_state) {
  if (current_state_ != new_state && IsValidTransition(new_state)) {
    current_state_ = new_state;

    switch (new_state) {
      case SnakeGameState::Ready:
        OnEnterReady();
        break;
      case SnakeGameState::Running:
        OnEnterRunning();
        break;
      case SnakeGameState::Paused:
        OnEnterPaused();
        break;
      case SnakeGameState::Won:
        OnEnterWon();
        break;
      case SnakeGameState::Lost:
        OnEnterLost();
        break;
    }
  }
}

bool SnakeFSM::IsValidTransition(SnakeGameState new_state) const {
  switch (current_state_) {
    case SnakeGameState::Ready:
      return new_state == SnakeGameState::Running;

    case SnakeGameState::Running:
      return new_state == SnakeGameState::Paused ||
             new_state == SnakeGameState::Lost ||
             new_state == SnakeGameState::Won;

    case SnakeGameState::Paused:
      return new_state == SnakeGameState::Running ||
             new_state == SnakeGameState::Lost;

    case SnakeGameState::Lost:
    case SnakeGameState::Won:
      return new_state == SnakeGameState::Ready;

    default:
      return false;
  }
}

void SnakeFSM::OnEnterReady() {
  game_.StartGame();
  game_.SetReady();
}

void SnakeFSM::OnEnterRunning() {
  game_.SetRunning();
}

void SnakeFSM::OnEnterPaused() {
  game_.SetPaused();
}

void SnakeFSM::OnEnterWon() {
  game_.SetWon();
}

void SnakeFSM::OnEnterLost() {
  game_.SetLost();
}

void SnakeFSM::UpdateState() {
  SnakeGameState game_state = game_.GetState();

  if (game_state != current_state_ && (game_state == SnakeGameState::Won ||
                                       game_state == SnakeGameState::Lost)) {
    current_state_ = game_state;
  }
}

}  // namespace s21
