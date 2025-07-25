#ifndef S21_SNAKE_GAME_HPP
#define S21_SNAKE_GAME_HPP

#include <array>
#include <deque>
#include <map>
#include <random>
#include <utility>

#include "../common/game_constants.h"
#include "../common/types.h"

namespace s21 {

enum class SnakeDirection { Up, Down, Left, Right };
enum class SnakeGameState { Ready, Running, Paused, Won, Lost };

struct SnakeSegment {
  int x;
  int y;
};

class SnakeGame {
 public:
  SnakeGame();
  void Reset();
  void Update();
  void ChangeDirection(UserAction_t action);
  void Accelerate(bool enable);
  void Pause();
  void Resume();
  void Terminate();

  GameInfo_t GetGameInfo() const;

  SnakeGameState GetState() const;

 private:
  void Move();
  void PlaceApple();
  bool CheckCollision(int x, int y) const;
  bool IsOppositeDirection(SnakeDirection dir) const;

  // Новый метод
  void Tick();

  void ClearField();
  void InitializeSnake();
  void UpdateHighScore();
  void FreeGameInfo(GameInfo_t& info) const;

  std::deque<SnakeSegment> snake_;
  SnakeDirection direction_;
  SnakeDirection next_direction_;
  SnakeGameState state_;
  int length_;
  int apple_x_;
  int apple_y_;
  int score_;
  int high_score_;
  int level_;
  int speed_;
  bool accelerated_;

  int field_[kGameHeight][kGameWidth];
  std::mt19937 gen_;
};

}  // namespace s21

#endif  // S21_SNAKE_GAME_HPP