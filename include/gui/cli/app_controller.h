/**
 * @file app_controller.h
 * @brief Контроллер приложений BrickGame.
 *
 * Определяет интерфейс для управления игровыми процессами (Tetris, Snake и
 * др.). Контроллер связывает ввод пользователя (`UserAction_t`) и логику
 * выбранной игры, управляет её состоянием и обеспечивает взаимодействие с
 * frontend (CLI, Qt).
 *
 * Используется в качестве центральной точки маршрутизации: принимает команды от
 * интерфейса, делегирует их игровой логике и возвращает обновлённое состояние
 * для отрисовки.
 */

#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <stdbool.h>

#include "../../brickgame/common/types.h"

/**
 * @enum GameType
 * @brief Перечисление поддерживаемых игр.
 */
typedef enum { GAME_TETRIS, GAME_SNAKE } GameType;

/**
 * @struct GameAPI
 * @brief Структура для работы с API динамически подключаемой игровой
 * библиотеки.
 */

typedef struct {
  void* lib_handle; /**< Дескриптор загруженной библиотеки */
  void (*userInput)(UserAction_t action,
                    bool hold); /**< Указатель на функцию обработки ввода */
  GameInfo_t (*updateState)(
      void);            /**< Указатель на функцию обновления состояния игры */
  bool (*isOver)(void); /**< Указатель на функцию проверки завершения игры */
  bool valid;           /**< Флаг валидности API */
  char* error;          /**< Сообщение об ошибке (если есть) */
} GameAPI;

/**
 * @brief Запускает приложение BrickGame.
 *
 * Отвечает за инициализацию ncurses, выбор игры,
 * загрузку библиотеки и запуск игрового цикла.
 */
void run_app(void);

/**
 * @brief Загружает динамическую библиотеку выбранной игры.
 *
 * @param game Тип игры (Tetris или Snake)
 * @return Структура GameAPI с указателями на функции игры или ошибкой
 */
GameAPI load_game_lib(GameType game);

/**
 * @brief Выгружает игровую библиотеку и освобождает ресурсы.
 *
 * @param api Структура GameAPI для выгрузки
 */
void unload_game_lib(GameAPI api);

#endif  // APP_CONTROLLER_H