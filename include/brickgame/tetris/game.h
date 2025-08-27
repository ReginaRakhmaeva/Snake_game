/**
 * @file game.h
 * @brief Внешний API для игры Tetris (BrickGame).
 *
 * Модуль предоставляет функции для работы с игрой из frontend'а (CLI, GUI).
 * Вся внутренняя логика реализована в backend и управляется FSM.
 *
 * Основные задачи API:
 * - обработка пользовательского ввода;
 * - обновление состояния игры на каждом тике;
 * - предоставление данных для отображения (GameInfo_t);
 * - определение окончания игры.
 */
#ifndef S21_TETRIS_GAME_H
#define S21_TETRIS_GAME_H

#include <stdbool.h>

#include "../common/types.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Обрабатывает ввод пользователя.
 *
 * Передаёт действие в FSM и backend для изменения состояния игры.
 *
 * @param action действие пользователя (влево, вправо, поворот и т.д.)
 * @param hold   признак удержания кнопки
 */
EXPORT void userInput(UserAction_t action, bool hold);
/**
 * @brief Обновляет текущее состояние игры.
 *
 * Синхронизирует структуру GameInfo_t с backend,
 * выполняет падение фигур и проверку завершения игры.
 *
 * @return GameInfo_t структура с данными для отрисовки.
 */
EXPORT GameInfo_t updateCurrentState();
/**
 * @brief Проверяет, окончена ли игра.
 *
 * @return true если игра завершена, иначе false.
 */
EXPORT bool isGameOver();

#ifdef __cplusplus
}
#endif

#endif  // S21_TETRIS_GAME_H