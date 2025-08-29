/**
 * @file snake_game.cpp
 * @brief Реализация основной логики игры Snake.
 *
 * Этот модуль содержит:
 * - управление состоянием игры (SnakeGameState);
 * - обновление поля и змейки;
 * - генерацию яблок;
 * - обработку столкновений и победы/поражения;
 * - сохранение и загрузку рекорда.
 */
#include "../../../include/brickgame/snake/snake_game.hpp"

#include <algorithm>
#include <fstream>
#include <random>
#include <string>

#include "../../../include/brickgame/common/types.h"

namespace s21 {

/**
 * @brief Конструктор SnakeGame.
 *
 * Загружает сохранённый рекорд и инициализирует состояние игры.
 */
SnakeGame::SnakeGame() : gen_(std::random_device{}()) {
  high_score_ = LoadHighScore();
  Reset();
}

/**
 * @brief Полная перезагрузка игры (сброс счёта, уровня, змейки и поля).
 */
void SnakeGame::Reset() {
  state_ = SnakeGameState::Ready;
  direction_ = SnakeDirection::Right;
  next_direction_ = SnakeDirection::Right;
  length_ = 4;
  score_ = 0;
  level_ = 1;
  speed_ = 600;
  accelerated_ = false;

  ClearField();
}
/**
 * @brief Очистка игрового поля (все клетки → пустые).
 */
void SnakeGame::ClearField() {
  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      field_[y][x] = static_cast<int>(CellType::Empty);
    }
  }
}
/**
 * @brief Размещение змейки в начальном положении.
 */
void SnakeGame::InitializeSnake() {
  snake_.clear();
  int start_x = 0;
  int start_y = kGameHeight / 2;

  for (int i = length_ - 1; i >= 0; --i) {
    snake_.push_back({start_x + i, start_y});
    field_[start_y][start_x + i] = static_cast<int>(CellType::Snake);
  }
}
/**
 * @brief Основное обновление логики (ход змейки, генерация яблок, ускорение).
 * 
 * Выполняет игровую логику, НЕ изменяет состояние игры.
 * Состояние управляется через FSM.
 */
void SnakeGame::Update() {
  if (state_ != SnakeGameState::Running) return;

  direction_ = next_direction_;
  Move();

  int baseSpeed = 600 - (level_ - 1) * 40;
  speed_ = accelerated_ ? baseSpeed / 2 : baseSpeed;
}
/**
 * @brief Изменить направление движения змейки (с проверкой на валидность).
 */
void SnakeGame::ChangeDirection(UserAction_t action, bool hold) {
  if (state_ != SnakeGameState::Running) return;

  SnakeDirection new_direction = direction_;

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
      return;
  }

  if (!IsOppositeDirection(new_direction)) {
    next_direction_ = new_direction;

    if (hold) {
      accelerated_ = true;
    } else {
      accelerated_ = false;
    }
  }
}
/**
 * @brief Передвигает змейку на один шаг.
 *
 * Проверяет столкновения, поедание яблок, рост змейки и победу.
 */
void SnakeGame::Move() {
  if (snake_.empty()) return;

  SnakeSegment head = CalculateNewHeadPosition();

  if (CheckCollisions(head)) {
    HandleCollision();
    return;
  }

  bool grow = CheckAppleEaten(head);

  UpdateSnake(head, grow);

  if (grow) {
    HandleAppleEaten();
  }
}
/**
 * @brief Разместить яблоко в случайной пустой клетке.
 */
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

/**
 * @brief Обновить и сохранить рекорд, если текущий счёт выше.
 */
void SnakeGame::UpdateHighScore() {
  if (score_ > high_score_) {
    high_score_ = score_;
    SaveHighScore();
  }
}
/**
 * @brief Получить текущее состояние игры в формате GameInfo_t.
 *
 * Память для поля выделяется динамически. Освобождение через FreeGameInfo().
 */
GameInfo_t SnakeGame::GetGameInfo() const {
  GameInfo_t info{};

  info.field = new int*[kGameHeight];
  for (int y = 0; y < kGameHeight; ++y) {
    info.field[y] = new int[kGameWidth];
    for (int x = 0; x < kGameWidth; ++x) {
      info.field[y][x] = field_[y][x];
    }
  }

  info.score = score_;
  info.high_score = high_score_;
  info.level = level_;
  info.speed = speed_;
  info.pause = (state_ == SnakeGameState::Paused) ? 1 : 0;
  info.next = nullptr;

  return info;
}

/**
 * @brief Освобождает память, выделенную для GameInfo_t::field.
 */
void SnakeGame::FreeGameInfo(GameInfo_t& info) const {
  if (info.field) {
    for (int y = 0; y < kGameHeight; ++y) {
      delete[] info.field[y];
    }
    delete[] info.field;
    info.field = nullptr;
  }
}
/**
 * \brief Проверяет, занята ли ячейка змейкой.
 * \param x Координата X.
 * \param y Координата Y.
 * \return true, если в ячейке находится часть змейки.
 */
bool SnakeGame::CheckCollision(int x, int y) const {
  return field_[y][x] == static_cast<int>(CellType::Snake);
}
/**
 * \brief Проверяет, является ли новое направление противоположным текущему.
 * \param dir Новое направление.
 * \return true, если направление противоположно текущему.
 */
bool SnakeGame::IsOppositeDirection(SnakeDirection dir) const {
  return (direction_ == SnakeDirection::Up && dir == SnakeDirection::Down) ||
         (direction_ == SnakeDirection::Down && dir == SnakeDirection::Up) ||
         (direction_ == SnakeDirection::Left && dir == SnakeDirection::Right) ||
         (direction_ == SnakeDirection::Right && dir == SnakeDirection::Left);
}


/**
 * \brief Управляет ускорением змейки.
 * \param enable true — включить ускорение, false — выключить.
 */
void SnakeGame::Accelerate(bool enable) { accelerated_ = enable; }
/**
 * \brief Обрабатывает один игровой тик.
 * Если игра в состоянии Running — выполняет Update().
 */
void SnakeGame::Tick() {
  if (state_ == SnakeGameState::Running) {
    Update();
  }
}
/**
 * \brief Загружает рекорд из файла snake_highscore.txt.
 * \return Сохранённый high score или 0, если файла нет.
 */
int SnakeGame::LoadHighScore() {
  std::ifstream file("snake_highscore.txt");
  int hs = 0;
  if (file.is_open()) {
    file >> hs;
    file.close();
  }
  return hs;
}
/**
 * \brief Сохраняет текущий рекорд в файл snake_highscore.txt.
 */
void SnakeGame::SaveHighScore() const {
  std::ofstream file("snake_highscore.txt");
  if (file.is_open()) {
    file << high_score_;
    file.close();
  }
}
/**
 * @brief Конструктор копирования.
 */
SnakeGame::SnakeGame(const SnakeGame& other)
    : snake_(other.snake_),
      direction_(other.direction_),
      next_direction_(other.next_direction_),
      state_(other.state_),
      length_(other.length_),
      apple_x_(other.apple_x_),
      apple_y_(other.apple_y_),
      score_(other.score_),
      high_score_(other.high_score_),
      level_(other.level_),
      speed_(other.speed_),
      accelerated_(other.accelerated_),
      gen_(other.gen_) {
  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      field_[y][x] = other.field_[y][x];
    }
  }
}

/**
 * @brief Оператор присваивания копированием.
 */
SnakeGame& SnakeGame::operator=(const SnakeGame& other) {
  if (this != &other) {
    snake_ = other.snake_;
    direction_ = other.direction_;
    next_direction_ = other.next_direction_;
    state_ = other.state_;
    length_ = other.length_;
    apple_x_ = other.apple_x_;
    apple_y_ = other.apple_y_;
    score_ = other.score_;
    high_score_ = other.high_score_;
    level_ = other.level_;
    speed_ = other.speed_;
    accelerated_ = other.accelerated_;
    gen_ = other.gen_;

    for (int y = 0; y < kGameHeight; ++y) {
      for (int x = 0; x < kGameWidth; ++x) {
        field_[y][x] = other.field_[y][x];
      }
    }
  }
  return *this;
}

/**
 * @brief Конструктор перемещения.
 */
SnakeGame::SnakeGame(SnakeGame&& other) noexcept
    : snake_(std::move(other.snake_)),
      direction_(other.direction_),
      next_direction_(other.next_direction_),
      state_(other.state_),
      length_(other.length_),
      apple_x_(other.apple_x_),
      apple_y_(other.apple_y_),
      score_(other.score_),
      high_score_(other.high_score_),
      level_(other.level_),
      speed_(other.speed_),
      accelerated_(other.accelerated_),
      gen_(std::move(other.gen_)) {
  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      field_[y][x] = other.field_[y][x];
    }
  }

  other.Reset();
}

/**
 * @brief Оператор присваивания перемещением.
 */
SnakeGame& SnakeGame::operator=(SnakeGame&& other) noexcept {
  if (this != &other) {
    snake_ = std::move(other.snake_);
    direction_ = other.direction_;
    next_direction_ = other.next_direction_;
    state_ = other.state_;
    length_ = other.length_;
    apple_x_ = other.apple_x_;
    apple_y_ = other.apple_y_;
    score_ = other.score_;
    high_score_ = other.high_score_;
    level_ = other.level_;
    speed_ = other.speed_;
    accelerated_ = other.accelerated_;
    gen_ = std::move(other.gen_);

    for (int y = 0; y < kGameHeight; ++y) {
      for (int x = 0; x < kGameWidth; ++x) {
        field_[y][x] = other.field_[y][x];
      }
    }

    other.Reset();
  }
  return *this;
}
/**
 * @brief Вычисляет новые координаты головы змейки.
 *
 * Смещает голову в зависимости от текущего направления движения.
 * Если змейка пуста, возвращает (0,0).
 */
SnakeSegment SnakeGame::CalculateNewHeadPosition() const {
  if (snake_.empty()) {
    return {0, 0};
  }

  SnakeSegment head = snake_.front();

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

  return head;
}
/**
 * @brief Проверяет столкновения головы змейки.
 *
 * Смотрит выход за границы поля и пересечение с собственным телом.
 *
 * @param head новая позиция головы
 * @return true если произошло столкновение, иначе false
 */
bool SnakeGame::CheckCollisions(const SnakeSegment& head) const {
  if (head.x < 0 || head.x >= kGameWidth || head.y < 0 ||
      head.y >= kGameHeight) {
    return true;
  }

  return CheckCollision(head.x, head.y);
}
/**
 * @brief Обрабатывает столкновение змейки.
 *
 * Переводит игру в состояние проигрыша и обновляет рекорд.
 */
void SnakeGame::HandleCollision() {
  state_ = SnakeGameState::Lost;
  UpdateHighScore();
}
/**
 * @brief Проверяет, съедено ли яблоко.
 *
 * Сравнивает координаты головы змейки с позицией яблока.
 *
 * @param head новая позиция головы змейки
 * @return true если яблоко съедено, иначе false
 */
bool SnakeGame::CheckAppleEaten(const SnakeSegment& head) const {
  return (head.x == apple_x_ && head.y == apple_y_);
}
/**
 * @brief Обрабатывает событие съедания яблока.
 *
 * Увеличивает длину змейки, добавляет очки, обновляет рекорд,
 * проверяет условие победы и устанавливает новый уровень.
 * При необходимости размещает новое яблоко.
 */
void SnakeGame::HandleAppleEaten() {
  ++length_;
  score_ += 1;
  UpdateHighScore();

  if (length_ >= kMaxSnakeLength) {
    state_ = SnakeGameState::Won;
    UpdateHighScore();
    return;
  }

  level_ = std::min(1 + score_ / 5, 10);
  PlaceApple();
}
/**
 * @brief Обновляет положение змейки на поле.
 *
 * Добавляет новую голову в начало, помечает её как часть змейки.
 * Если змейка не растёт — удаляет хвост и освобождает клетку.
 *
 * @param head Новая позиция головы змейки.
 * @param grow Флаг, указывающий, нужно ли увеличить длину змейки
 *             (true при поедании яблока).
 */
void SnakeGame::UpdateSnake(const SnakeSegment& head, bool grow) {
  snake_.push_front(head);
  field_[head.y][head.x] = static_cast<int>(CellType::Snake);

  if (!grow) {
    SnakeSegment tail = snake_.back();
    field_[tail.y][tail.x] = static_cast<int>(CellType::Empty);
    snake_.pop_back();
  }
}

/**
 * @brief Запускает новую игру.
 * 
 * Сбрасывает состояние и инициализирует змейку и яблоко.
 * Состояние устанавливается через FSM.
 */
void SnakeGame::StartGame() {
  Reset();
  InitializeSnake();
  PlaceApple();
  state_ = SnakeGameState::Running;
}

}  // namespace s21