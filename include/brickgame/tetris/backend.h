/**
 * @file backend.h
 * @brief Чистая игровая логика для Tetris (backend).
 *
 * Этот модуль реализует внутреннюю механику Tetris:
 * - управление фигурами (движение, фиксация, падение);
 * - обновление игрового поля и проверка заполненных линий;
 * - система очков, уровней и скорости;
 * - сохранение и загрузка рекордов;
 * - подготовка данных для отображения (GameInfo_t).
 *
 * Backend полностью изолирован от интерфейса —
 * он работает только с игровым состоянием и данными.
 */
#ifndef BRICKGAME_TETRIS_BACKEND_H_
#define BRICKGAME_TETRIS_BACKEND_H_

#include <stdio.h>
#define SCORE_FILE "tetris_highscore.txt"
#include "../common/types.h"

/**
 * @brief Статус выполнения игровой операции.
 */
typedef enum { BACKEND_OK, BACKEND_GAME_OVER } BackendStatus;

/**
 * @brief Инициализация новой игры или рестарт.
 *
 * @return GameInfo_t структура с начальными данными игры.
 */
GameInfo_t backend_init_game(void);
/**
 * @brief Обработка пользовательского ввода.
 *
 * @param action действие пользователя (влево, вправо, вращение и т.д.)
 * @param hold   признак удержания кнопки
 * @return BackendStatus (OK или GAME_OVER)
 */
BackendStatus backend_handle_input(UserAction_t action, bool hold);

/**
 * @brief Обновляет физику игры (падение фигуры по таймеру).
 *
 * @param info указатель на структуру состояния игры
 * @return BackendStatus (OK или GAME_OVER)
 */
BackendStatus backend_update_physics(GameInfo_t *info);
/**
 * @brief Фиксирует текущую фигуру на поле.
 *
 * @return BackendStatus (OK или GAME_OVER)
 */
BackendStatus backend_fix_piece(void);

/**
 * @brief Возвращает задержку (скорость) для указанного уровня.
 *
 * @param level уровень игры
 * @return скорость (в мс или тиках)
 */
int get_level_speed(int level);
/**
 * @brief Накладывает текущую фигуру на игровое поле (для отображения).
 *
 * @param info_ptr указатель на GameInfo_t
 */
void backend_overlay_piece(GameInfo_t *info_ptr);
/**
 * @brief Сохраняет лучший результат в файл.
 *
 * @param high_score значение рекорда
 */
void save_high_score(int high_score);
/**
 * @brief Получает актуальное состояние игры.
 *
 * @return GameInfo_t структура с данными для отображения.
 */
GameInfo_t backend_get_info(void);

/**
 * @brief Освобождает выделенные ресурсы (игровое поле и т.д.).
 *
 * @param game_info указатель на структуру с состоянием игры
 */
void backend_free_game_info(GameInfo_t *game_info);

#endif  // BRICKGAME_TETRIS_BACKEND_H_