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
   * @brief Обрабатывает пользовательский ввод и меняет состояние игры.
   *
   * @param action действие пользователя (Start, Pause, Terminate и т.д.)
   * @param hold признак удержания кнопки (актуально для ускорения).
   */
  void HandleInput(UserAction_t action, bool hold);

  /**
   * @brief Устанавливает состояние игры.
   * 
   * @param state новое состояние игры.
   */
  void SetState(SnakeGameState state);

  /**
   * @brief Запускает новую игру.
   */
  void StartGame();

  /**
   * @brief Ставит игру на паузу.
   */
  void PauseGame();

  /**
   * @brief Возобновляет игру.
   */
  void ResumeGame();

  /**
   * @brief Завершает игру.
   */
  void TerminateGame();

 private:
  /**
   * @brief Ссылка на управляемый объект игры Snake.
   */
  SnakeGame& game_;
};

}  // namespace s21

#endif  // S21_SNAKE_FSM_HPP