#include "../../../include/brickgame/snake/snake_game.hpp"

#include <algorithm>
#include <random>
#include <fstream>
#include <string>

namespace s21 {

// Константы для типов клеток
enum class CellType { Empty = 0, Snake = 1, Apple = 2 };

// Константа для максимальной длины змейки
constexpr int kMaxSnakeLength = 200;

SnakeGame::SnakeGame() : gen_(std::random_device{}()) {
    high_score_ = LoadHighScore();
    Reset();
}

void SnakeGame::Reset() {
  state_ = SnakeGameState::Ready;
  direction_ = SnakeDirection::Right;
  next_direction_ = SnakeDirection::Right;
  length_ = 4;
  score_ = 0;
  level_ = 1;
  speed_ = 600;  // как в тетрисе
  accelerated_ = false;

  ClearField();
}

void SnakeGame::ClearField() {
  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      field_[y][x] = static_cast<int>(CellType::Empty);
    }
  }
}

void SnakeGame::InitializeSnake() {
  snake_.clear();
  int start_x = kGameWidth / 2 - 2;
  int start_y = kGameHeight / 2;

  for (int i = length_ - 1; i >= 0; --i) {
    snake_.push_back({start_x + i, start_y});
    field_[start_y][start_x + i] = static_cast<int>(CellType::Snake);
  }
}

void SnakeGame::Update() {
  if (state_ == SnakeGameState::Ready) {
    // При первом обновлении инициализируем змейку
    InitializeSnake();
    PlaceApple();
    state_ = SnakeGameState::Running;
    return;
  }

  if (state_ != SnakeGameState::Running) return;

  direction_ = next_direction_;
  Move();

  speed_ = 600 - (level_ - 1) * 60;
  if (speed_ < 80) speed_ = 80;
}

void SnakeGame::ChangeDirection(UserAction_t action) {
  if (state_ != SnakeGameState::Running) return;

  SnakeDirection new_direction = direction_;  // по умолчанию не меняем

  switch (action) {
    case Up:
      new_direction = SnakeDirection::Up;
      break;
    case Down:
      new_direction = SnakeDirection::Down;
      break;
    case Left:
      new_direction = SnakeDirection::Left;
      break;
    case Right:
      new_direction = SnakeDirection::Right;
      break;
    default:
      return;  // игнорируем другие действия
  }

  // Проверяем, что новое направление не противоположно текущему
  if (!IsOppositeDirection(new_direction)) {
    next_direction_ = new_direction;
  }
}

void SnakeGame::Move() {
  if (snake_.empty()) return;

  SnakeSegment head = snake_.front();
  // ВРЕМЕННЫЙ ОТЛАДОЧНЫЙ ВЫВОД
  FILE* dbg = fopen("debug.log", "a");
  if (dbg) {
    fprintf(dbg, "Move: old head=(%d,%d) dir=%d\n", head.x, head.y,
            (int)direction_);
  }
  // КОНЕЦ ОТЛАДКИ

  switch (direction_) {
    case SnakeDirection::Up:
      head.y -= 1;
      break;
    case SnakeDirection::Down:
      head.y += 1;
      break;
    case SnakeDirection::Left:
      head.x -= 1;
      break;
    case SnakeDirection::Right:
      head.x += 1;
      break;
  }

  // ВРЕМЕННЫЙ ОТЛАДОЧНЫЙ ВЫВОД
  if (dbg) {
    fprintf(dbg, "Move: new head=(%d,%d)\n", head.x, head.y);
  }
  // КОНЕЦ ОТЛАДКИ

  // Проверка столкновений
  bool collision = (head.x < 0 || head.x >= kGameWidth || head.y < 0 ||
                    head.y >= kGameHeight || CheckCollision(head.x, head.y));
  if (dbg) {
    fprintf(dbg, "Move: collision=%d\n", collision ? 1 : 0);
  }
  if (collision) {
    state_ = SnakeGameState::Lost;
    UpdateHighScore();
    if (dbg) {
      fprintf(dbg, "Move: GAME OVER!\n");
      fclose(dbg);
    }
    return;
  }

  // Проверка яблока
  bool grow = (head.x == apple_x_ && head.y == apple_y_);
  snake_.push_front(head);
  field_[head.y][head.x] = static_cast<int>(CellType::Snake);

  if (grow) {
    ++length_;
    score_ += 1;
    UpdateHighScore(); // <--- ВЫЗЫВАТЬ ВСЕГДА при поедании яблока!
    if (length_ >= kMaxSnakeLength) {
      state_ = SnakeGameState::Won;
      UpdateHighScore();
      if (dbg) {
        fprintf(dbg, "Move: WIN!\n");
        fclose(dbg);
      }
      return;
    }
    level_ = std::min(1 + score_ / 5, 10);
    PlaceApple();
  } else {
    SnakeSegment tail = snake_.back();
    field_[tail.y][tail.x] = static_cast<int>(CellType::Empty);
    snake_.pop_back();
  }
}

void SnakeGame::PlaceApple() {
  std::uniform_int_distribution<int> dist_x(0, kGameWidth - 1);
  std::uniform_int_distribution<int> dist_y(0, kGameHeight - 1);

  int x, y;
  do {
    x = dist_x(gen_);
    y = dist_y(gen_);
  } while (field_[y][x] != static_cast<int>(CellType::Empty));

  apple_x_ = x;
  apple_y_ = y;
  field_[y][x] = static_cast<int>(CellType::Apple);
}

void SnakeGame::UpdateHighScore() {
    if (score_ > high_score_) {
        high_score_ = score_;
        SaveHighScore();
    }
}

GameInfo_t SnakeGame::GetGameInfo() const {
  GameInfo_t info{};

  // Выделяем память под поле
  info.field = new int*[kGameHeight];
  for (int y = 0; y < kGameHeight; ++y) {
    info.field[y] = new int[kGameWidth];
    for (int x = 0; x < kGameWidth; ++x) {
      info.field[y][x] = field_[y][x];
    }
  }

  // ВРЕМЕННЫЙ ОТЛАДОЧНЫЙ ВЫВОД
  FILE* dbg = fopen("debug.log", "a");
  if (dbg) {
    fprintf(dbg, "GetGameInfo field_ state:\n");
    for (int y = 0; y < kGameHeight; ++y) {
      for (int x = 0; x < kGameWidth; ++x) {
        fprintf(dbg, "%d", field_[y][x]);
      }
      fprintf(dbg, "\n");
    }
    fprintf(dbg, "---\n");
    fclose(dbg);
  }
  // КОНЕЦ ОТЛАДКИ

  info.score = score_;
  info.high_score = high_score_;
  info.level = level_;
  info.speed = speed_;
  info.pause = (state_ == SnakeGameState::Paused) ? 1 : 0;
  info.next = nullptr;  // У змейки нет next-фигуры, явно указать

  return info;
}

void SnakeGame::FreeGameInfo(GameInfo_t& info) const {
  if (info.field) {
    for (int y = 0; y < kGameHeight; ++y) {
      delete[] info.field[y];
    }
    delete[] info.field;
    info.field = nullptr;
  }
}

bool SnakeGame::CheckCollision(int x, int y) const {
  return field_[y][x] == static_cast<int>(CellType::Snake);
}

bool SnakeGame::IsOppositeDirection(SnakeDirection dir) const {
  return (direction_ == SnakeDirection::Up && dir == SnakeDirection::Down) ||
         (direction_ == SnakeDirection::Down && dir == SnakeDirection::Up) ||
         (direction_ == SnakeDirection::Left && dir == SnakeDirection::Right) ||
         (direction_ == SnakeDirection::Right && dir == SnakeDirection::Left);
}

SnakeGameState SnakeGame::GetState() const { return state_; }

void SnakeGame::Resume() {
  if (state_ == SnakeGameState::Paused || state_ == SnakeGameState::Ready) {
    if (state_ == SnakeGameState::Ready) {
      Update();  // инициализация змейки и яблока
    }
    state_ = SnakeGameState::Running;
  }
}

void SnakeGame::Pause() {
  if (state_ == SnakeGameState::Running) state_ = SnakeGameState::Paused;
}

void SnakeGame::Terminate() { state_ = SnakeGameState::Lost; }

void SnakeGame::Accelerate(bool enable) { accelerated_ = enable; }

void SnakeGame::Tick() {
  if (state_ == SnakeGameState::Running) {
    Update();  // Просто вызываем обновление
  }
}

int SnakeGame::LoadHighScore() {
    std::ifstream file("snake_highscore.txt");
    int hs = 0;
    if (file.is_open()) {
        file >> hs;
        file.close();
    }
    return hs;
}

void SnakeGame::SaveHighScore() const {
    std::ofstream file("snake_highscore.txt");
    if (file.is_open()) {
        file << high_score_;
        file.close();
        // ВРЕМЕННЫЙ ОТЛАДОЧНЫЙ ВЫВОД
        FILE* dbg = fopen("debug1.log", "a");
        if (dbg) {
            fprintf(dbg, "SaveHighScore: saved %d\n", high_score_);
            fclose(dbg);
        }
    }
}

}  // namespace s21