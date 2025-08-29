/**
 * @file snake_fsm.hpp
 * @brief Определение конечного автомата для управления игрой Snake.
 *
 * Этот модуль реализует класс SnakeFSM, который связывает пользовательский ввод
 * (UserAction_t) с логикой игры SnakeGame. FSM интерпретирует нажатия и
 * удержания клавиш и преобразует их в команды игры: изменение направления,
 * постановка на паузу, завершение игры или ускорение движения змейки.
 *
 * Используется совместно с:
 * - SnakeGame (внутренняя игровая логика)
 * - snake_api.h (внешнее API для frontend части)
 */
#ifndef S21_SNAKE_FSM_HPP
#define S21_SNAKE_FSM_HPP

#include "snake_game.hpp"

namespace s21 {
/**
 * @brief Класс конечного автомата для игры Snake.
 *
 * SnakeFSM отвечает за обработку пользовательского ввода и перевод его
 * в игровые действия. Он связывает внешнее API (userInput) с внутренней
 * логикой SnakeGame.
 */
class SnakeFSM {
 public:
  /**
   * @brief Конструктор конечного автомата.
   * @param game Ссылка на объект SnakeGame, над которым выполняются действия.
   */
  explicit SnakeFSM(SnakeGame& game);
  /**
   * @brief Обрабатывает действие пользователя.
   *
   * На основе события (нажатия клавиши или удержания) вызывает
   * соответствующие методы SnakeGame:
   * - Start — сброс и запуск игры
   * - Pause — пауза/возобновление
   * - Terminate — завершение игры
   * - Up/Down/Left/Right — изменение направления движения
   * - Action — ускорение (удержание клавиши)
   *
   * @param action Действие пользователя (см. UserAction_t).
   * @param hold Флаг удержания клавиши (используется для ускорения).
   */
  void HandleInput(UserAction_t action, bool hold);

 private:
  /**
   * @brief Ссылка на управляемый объект игры Snake.
   */
  SnakeGame& game_;
};

}  // namespace s21

#endif  // S21_SNAKE_FSM_HPP