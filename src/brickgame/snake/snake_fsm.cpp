/**
 * @file snake_fsm.cpp
 * @brief Реализация конечного автомата (FSM) для игры Snake.
 *
 * FSM отвечает за обработку пользовательского ввода и
 * управление состоянием игры Snake (через SnakeGame).
 *
 * Основные задачи:
 * - запуск новой игры и рестарт;
 * - постановка игры на паузу и возобновление;
 * - завершение игры;
 * - изменение направления движения змейки;
 * - ускорение змейки при удержании кнопки.
 */
#include "../../../include/brickgame/snake/snake_fsm.hpp"

namespace s21 {

/**
 * @brief Конструктор FSM для Snake.
 *
 * @param game ссылка на SnakeGame, которым управляет FSM.
 */
SnakeFSM::SnakeFSM(SnakeGame& game) : game_(game) {}
/**
 * @brief Обрабатывает пользовательский ввод и меняет состояние игры.
 *
 * В зависимости от текущего состояния SnakeGame и действия пользователя
 * выполняются переходы и вызовы соответствующих методов:
 * - Start: запуск новой игры или рестарт после победы/поражения;
 * - Pause: пауза и возобновление;
 * - Terminate: завершение игры;
 * - Up/Down/Left/Right: смена направления змейки;
 * - Action: ускорение (при удержании кнопки).
 *
 * @param action действие пользователя (Start, Pause, Terminate и т.д.)
 * @param hold признак удержания кнопки (актуально для ускорения).
 */
void SnakeFSM::HandleInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      if (game_.GetState() == SnakeGameState::Ready ||
          game_.GetState() == SnakeGameState::Lost ||
          game_.GetState() == SnakeGameState::Won) {
        game_.Reset();
        game_.Resume();
      }
      break;
    case Pause:
      if (game_.GetState() == SnakeGameState::Running) {
        game_.Pause();
      } else if (game_.GetState() == SnakeGameState::Paused) {
        game_.Resume();
      }
      break;
    case Terminate:
      game_.Terminate();
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
}

}  // namespace s21
