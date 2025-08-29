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
   * @brief Обрабатывает пользовательский ввод и выполняет переходы состояний.
   *
   * @param action действие пользователя (Start, Pause, Terminate и т.д.)
   * @param hold признак удержания кнопки (актуально для ускорения).
   */
  void HandleInput(UserAction_t action, bool hold);

  /**
   * @brief Обрабатывает изменения состояния из игры (победа/поражение).
   * 
   * Вызывается после Update() для синхронизации состояния FSM с состоянием игры.
   */
  void UpdateState();

  /**
   * @brief Возвращает текущее состояние FSM.
   * 
   * @return текущее состояние игры.
   */
  SnakeGameState GetState() const { return current_state_; }

 private:
  /**
   * @brief Ссылка на управляемый объект игры Snake.
   */
  SnakeGame& game_;

  /**
   * @brief Текущее состояние FSM.
   */
  SnakeGameState current_state_;

  /**
   * @brief Выполняет переход в новое состояние.
   * 
   * @param new_state новое состояние.
   */
  void TransitionTo(SnakeGameState new_state);

  /**
   * @brief Проверяет, возможен ли переход в новое состояние.
   * 
   * @param new_state новое состояние.
   * @return true если переход валиден, иначе false.
   */
  bool IsValidTransition(SnakeGameState new_state) const;

  /**
   * @brief Выполняет действие при переходе в состояние Ready.
   */
  void OnEnterReady();

  /**
   * @brief Выполняет действие при переходе в состояние Running.
   */
  void OnEnterRunning();

  /**
   * @brief Выполняет действие при переходе в состояние Paused.
   */
  void OnEnterPaused();

  /**
   * @brief Выполняет действие при переходе в состояние Won.
   */
  void OnEnterWon();

  /**
   * @brief Выполняет действие при переходе в состояние Lost.
   */
  void OnEnterLost();
};

}  // namespace s21

#endif  // S21_SNAKE_FSM_HPP