/**
 * @file snake_game.hpp
 * @brief Класс SnakeGame — основная игровая логика «Змейки».
 *
 * Здесь содержится реализация всех игровых механик:
 * - хранение состояния поля и змейки;
 * - обработка движения, столкновений и роста змейки;
 * - управление скоростью, уровнями и очками;
 * - генерация яблок;
 * - сохранение и загрузка рекордов;
 * - поддержка игровых состояний (Ready, Running, Paused, Won, Lost).
 *
 */
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
  /**
   * @brief Конструктор. Загружает рекорд и инициализирует игру.
   */
  SnakeGame();

  /**
   * @brief Полный сброс игры (счёт, уровень, змейка, поле).
   */
  void Reset();

  /**
   * @brief Выполняет обновление состояния (движение, рост, яблоки).
   */
  void Update();

  /**
   * @brief Меняет направление движения змейки (если допустимо).
   * @param action Действие пользователя (Up, Down, Left, Right).
   */
  void ChangeDirection(UserAction_t action);

  /**
   * @brief Управляет ускорением змейки.
   * @param enable true — включить ускорение, false — выключить.
   */
  void Accelerate(bool enable);

  /**
   * @brief Ставит игру на паузу.
   */
  void Pause();

  /**
   * @brief Возобновляет игру (после паузы или из Ready).
   */
  void Resume();

  /**
   * @brief Принудительно завершает игру (Lost).
   */
  void Terminate();

  /**
   * @brief Возвращает полную информацию о текущем состоянии игры.
   * @return Структура GameInfo_t (поле, очки, уровень, скорость, пауза).
   */
  GameInfo_t GetGameInfo() const;

  /**
   * @brief Получает текущее состояние игры.
   * @return Ready, Running, Paused, Won или Lost.
   */
  SnakeGameState GetState() const;

  /**
   * @brief Освобождает память, выделенную в GetGameInfo().
   * @param info Структура для очистки.
   */
  void FreeGameInfo(GameInfo_t& info) const;

  /**
   * @brief Обрабатывает один игровой тик (выполняет Update при Running).
   */
  void Tick();

 private:
  /**
   * @brief Передвигает змейку на один шаг. Проверяет столкновения и рост.
   */
  void Move();

  /**
   * @brief Ставит яблоко в случайную пустую клетку.
   */
  void PlaceApple();

  /**
   * @brief Проверяет, находится ли в ячейке часть змейки.
   * @param x Координата X.
   * @param y Координата Y.
   * @return true, если в ячейке змейка.
   */
  bool CheckCollision(int x, int y) const;

  /**
   * @brief Проверяет, противоположно ли новое направление текущему.
   * @param dir Новое направление.
   * @return true, если противоположно.
   */
  bool IsOppositeDirection(SnakeDirection dir) const;

  /**
   * @brief Очищает поле (все клетки → пустые).
   */
  void ClearField();

  /**
   * @brief Размещает змейку в стартовом положении.
   */
  void InitializeSnake();

  /**
   * @brief Обновляет high score при необходимости и сохраняет его.
   */
  void UpdateHighScore();

  /**
   * @brief Загружает рекорд из файла snake_highscore.txt.
   * @return Значение рекорда или 0.
   */
  int LoadHighScore();

  /**
   * @brief Сохраняет рекорд в файл snake_highscore.txt.
   */
  void SaveHighScore() const;

  /**
   * @brief Текущее тело змейки, представленное последовательностью сегментов.
   * Первый элемент — голова, последний — хвост.
   */
  std::deque<SnakeSegment> snake_;

  /**
   * @brief Текущее направление движения змейки.
   */
  SnakeDirection direction_;

  /**
   * @brief Направление, которое змейка примет на следующем шаге.
   * Устанавливается через обработку ввода.
   */
  SnakeDirection next_direction_;

  /**
   * @brief Текущее состояние игры (готово, запущено, на паузе, победа,
   * поражение).
   */
  SnakeGameState state_;

  /**
   * @brief Текущая длина змейки.
   */
  int length_;

  /**
   * @brief X-координата яблока на игровом поле.
   */
  int apple_x_;

  /**
   * @brief Y-координата яблока на игровом поле.
   */
  int apple_y_;

  /**
   * @brief Очки игрока за текущую игру.
   */
  int score_;

  /**
   * @brief Рекордный счёт (high score).
   */
  int high_score_;

  /**
   * @brief Текущий уровень игры (увеличивается с ростом счёта).
   */
  int level_;

  /**
   * @brief Скорость игры (меньше — быстрее).
   */
  int speed_;

  /**
   * @brief Флаг ускоренного движения (включается при резких манёврах).
   */
  bool accelerated_;

  /**
   * @brief Количество последовательных ходов в одном и том же направлении.
   * Используется для активации ускорения.
   */
  int consecutive_moves_;

  /**
   * @brief Последнее направление движения, использованное змейкой.
   */
  SnakeDirection last_direction_;

  /**
   * @brief Игровое поле в виде матрицы (ячейки: пустая, змейка, яблоко).
   */
  int field_[kGameHeight][kGameWidth];

  /**
   * @brief Генератор случайных чисел для появления яблок.
   */
  std::mt19937 gen_;
};
}  // namespace s21

#endif  // S21_SNAKE_GAME_HPP