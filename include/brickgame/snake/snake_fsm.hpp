#ifndef S21_SNAKE_FSM_HPP
#define S21_SNAKE_FSM_HPP

#include "snake_game.hpp"

namespace s21 {

class SnakeFSM {
 public:
  explicit SnakeFSM(SnakeGame& game);

  void HandleInput(UserAction_t action, bool hold);

 private:
  SnakeGame& game_;
};

}  // namespace s21

#endif  // S21_SNAKE_FSM_HPP