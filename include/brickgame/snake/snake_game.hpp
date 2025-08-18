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
  // раньше был приватным
  void FreeGameInfo(GameInfo_t& info) const;
  // Новый метод
  void Tick();

 private:
  void Move();
  void PlaceApple();
  bool CheckCollision(int x, int y) const;
  bool IsOppositeDirection(SnakeDirection dir) const;

  void ClearField();
  void InitializeSnake();
  void UpdateHighScore();

  int LoadHighScore();
  void SaveHighScore() const;
  friend class SnakeGameTestHelper;

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

class SnakeGameTestHelper {
 public:
  // ==== Базовые геттеры ====
  static SnakeSegment GetHead(const SnakeGame& game) {
    return game.snake_.front();
  }

  static std::deque<SnakeSegment>& SnakeSegments(SnakeGame& game) {
    return game.snake_;
  }

  static int& CellAt(SnakeGame& game, int y, int x) {
    return game.field_[y][x];
  }

  static int GetAppleX(const SnakeGame& game) { return game.apple_x_; }
  static int GetAppleY(const SnakeGame& game) { return game.apple_y_; }
  static int GetSpeed(const SnakeGame& game) { return game.speed_; }
  static SnakeGameState GetState(const SnakeGame& game) { return game.state_; }

  // ==== Управление внутренними полями ====
  static void SetLength(SnakeGame& game, int length) { game.length_ = length; }
  static void SetState(SnakeGame& game, SnakeGameState state) {
    game.state_ = state;
  }

  // ==== Работа с яблоком ====
  static void ForceAppleAt(SnakeGame& game, int x, int y) {
    game.apple_x_ = x;
    game.apple_y_ = y;
    game.field_[y][x] = static_cast<int>(CellType::Apple);
  }
  // Принудительно выставить скорость (для теста)
  static void ForceSpeed(SnakeGame& game, int speed) { game.speed_ = speed; }

  static void CallPlaceApple(SnakeGame& game) { game.PlaceApple(); }

  // ==== Проверка коллизий ====
  static bool TestCollision(SnakeGame& game, int x, int y) {
    return game.CheckCollision(x, y);
  }

  // ==== Сценарные действия для тестов ====
  static void MoveSnakeToWall(SnakeGame& game) {
    // Двигаем змею до правой стены
    while (GetHead(game).x < kGameWidth - 1 &&
           game.GetState() == SnakeGameState::Running) {
      game.Tick();
    }
    // Следующий тик должен убить её
    game.Tick();
  }

  static void ExtendSnakeToWin(SnakeGame& game) {
    // Насильно делаем максимальную длину
    SnakeSegments(game).clear();
    for (int y = 0; y < kGameHeight; ++y) {
      for (int x = 0; x < kGameWidth; ++x) {
        SnakeSegments(game).push_back({x, y});
      }
    }
    SetLength(game, static_cast<int>(SnakeSegments(game).size()));
    SetState(game, SnakeGameState::Won);
  }

  static void ExtendSnake(SnakeGame& game, int n) {
    SnakeSegments(game).clear();
    for (int i = 0; i < n; i++) {
      SnakeSegments(game).push_back({i, 0});
    }
    SetLength(game, n);
  }
};
}  // namespace s21

#endif  // S21_SNAKE_GAME_HPP