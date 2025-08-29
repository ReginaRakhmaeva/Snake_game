
/**
 * @file snake_api.cpp
 * @brief Внешний API для игры Snake (BrickGame).
 *
 * Этот модуль связывает C++-реализацию игры Snake (SnakeGame и SnakeFSM)
 * с C-интерфейсом (extern "C"), который используется фронтендом.
 *
 * Основные задачи API:
 * - обработка пользовательского ввода;
 * - обновление состояния игры на каждом тике;
 * - предоставление данных для отрисовки (GameInfo_t);
 * - определение завершения игры (поражение или победа).
 */
#include "../../../include/brickgame/snake/snake_api.h"

#include "../../../include/brickgame/snake/snake_fsm.hpp"
#include "../../../include/brickgame/snake/snake_game.hpp"

namespace s21 {
static SnakeGame game;      ///< Экземпляр игры Snake
static SnakeFSM fsm(game);  ///< FSM для обработки ввода

}  // namespace s21
static GameInfo_t previous_info;  ///< Кэш предыдущего состояния
/**
 * @brief Обрабатывает ввод пользователя.
 *
 * Передаёт действие в SnakeFSM для изменения состояния игры.
 *
 * @param action действие пользователя (Up, Down, Left, Right, Pause и т.д.)
 * @param hold   признак удержания кнопки
 */
extern "C" EXPORT void userInput(UserAction_t action, bool hold) {
  s21::fsm.HandleInput(action, hold);
}
/**
 * @brief Обновляет состояние игры Snake.
 *
 * Вызывает тик игрового цикла, синхронизирует состояние FSM,
 * освобождает предыдущее состояние, получает новое GameInfo_t для отображения.
 *
 * @return GameInfo_t структура с данными для отрисовки.
 */
extern "C" EXPORT GameInfo_t updateCurrentState() {
  s21::game.Tick();
  s21::fsm.UpdateState();
  s21::game.FreeGameInfo(previous_info);
  previous_info = s21::game.GetGameInfo();
  return previous_info;
}
/**
 * @brief Проверяет, окончена ли игра (поражение или победа).
 *
 * @return true если Snake проиграл или победил, иначе false.
 */
extern "C" EXPORT bool isGameOver() {
  auto state = s21::fsm.GetState();
  return state == s21::SnakeGameState::Lost ||
         state == s21::SnakeGameState::Won;
}
/**
 * @brief Проверяет, выиграл ли игрок.
 *
 * @return true если игра окончилась победой, иначе false.
 */

extern "C" EXPORT bool isVictory() {
  return s21::fsm.GetState() == s21::SnakeGameState::Won;
}
