/**
 * @file snake_api.h
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
 */ \
#ifndef S21_SNAKE_API_H
#define S21_SNAKE_API_H

#include <stdbool.h>

#include "../common/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Проверяет, выиграл ли игрок.
 * \return true, если игрок победил, иначе false.
 */
EXPORT bool isVictory(void);

/**
 * \brief Обрабатывает ввод пользователя и передаёт его в игру.
 * \param action действие пользователя (клавиша).
 * \param hold   флаг удержания действия (например, ускорение).
 */
EXPORT void userInput(UserAction_t action, bool hold);
/**
 * \brief Обновляет текущее состояние игры и возвращает структуру для отрисовки.
 * \return GameInfo_t актуальное состояние игрового поля и статистики.
 */
EXPORT GameInfo_t updateCurrentState();
/**
 * \brief Проверяет, окончена ли игра (проигрыш).
 * \return true, если игра окончена, иначе false.
 */
EXPORT bool isGameOver();

#ifdef __cplusplus
}
#endif

#endif  // S21_SNAKE_API_H