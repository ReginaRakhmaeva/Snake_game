/**
 * @file types.h
 * @brief Общие типы и структуры для игр проекта BrickGame.
 *
 * Этот заголовочный файл определяет основные перечисления, структуры
 * и функции API, используемые как Тетрисом, так и другими играми
 * (например, Змейкой).
 *
 * Здесь находятся:
 * - перечисление пользовательских действий (UserAction_t);
 * - структура состояния игры (GameInfo_t);
 * - универсальный тип клеток игрового поля (CellType);
 * - базовый API для обработки ввода и получения состояния.
 *
 * Файл служит связующим интерфейсом между бэкендом игр
 * и фронтендом (CLI, GUI и т.д.).
 */
#ifndef BRICKGAME_COMMON_TYPES_H
#define BRICKGAME_COMMON_TYPES_H

#include <stdbool.h>

// Макрос для экспорта функций в разделяемых библиотеках
#define EXPORT __attribute__((visibility("default")))

/**
 * @brief Возможные действия пользователя.
 */
typedef enum {
  Start,      ///< Начало игры
  Pause,      ///< Пауза
  Terminate,  ///< Завершение
  Left,       ///< Движение влево
  Right,      ///< Движение вправо
  Up,         ///< Движение вверх
  Down,       ///< Движение вниз
  Action  ///< Действие (например, поворот фигуры или "укус")
} UserAction_t;

/**
 * @brief Структура с данными о текущем состоянии игры.
 */
typedef struct {
  int **field;  ///< Игровое поле (двумерный массив)
  int **next;   ///< Следующая фигура / объект
  int score;    ///< Текущий счёт
  int high_score;  ///< Рекорд
  int level;       ///< Текущий уровень
  int speed;       ///< Скорость игры
  int pause;       ///< Флаг паузы
} GameInfo_t;

#ifdef __cplusplus
/**
 * @brief Типы клеток игрового поля (используется в Snake).
 */
enum class CellType { Empty = 0, Snake = 1, Apple = 2 };
#else
typedef enum { Empty = 0, Snake = 1, Apple = 2 } CellType;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Обрабатывает действие пользователя.
 * @param action Действие (см. UserAction_t)
 * @param hold Флаг удержания кнопки
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Возвращает текущее состояние игры.
 * @return GameInfo_t структура с данными
 */
GameInfo_t updateCurrentState();

#ifdef __cplusplus
}
#endif

#endif  // BRICKGAME_COMMON_TYPES_H
