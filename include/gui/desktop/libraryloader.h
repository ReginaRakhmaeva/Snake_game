/**
 * @file libraryloader.h
 * @brief Класс для динамической подгрузки игровых библиотек (Tetris, Snake)
 * через dlopen/dlsym и предоставления API для взаимодействия с игрой.
 */
#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include <dlfcn.h>

#include <QObject>
#include <QString>

#include "../../brickgame/common/types.h"

/**
 * @brief Перечисление доступных типов игр.
 */
enum class GameType { TETRIS, SNAKE };

/**
 * @brief Структура для хранения указателей на функции игры из динамической
 * библиотеки.
 */
struct GameAPI {
  void* lib_handle = nullptr; /**< Хэндл библиотеки */
  void (*userInput)(UserAction_t action,
                    bool hold) = nullptr; /**< Функция обработки ввода */
  GameInfo_t (*updateCurrentState)(void) =
      nullptr; /**< Функция получения текущего состояния игры */
  bool (*isOver)(void) = nullptr; /**< Функция проверки окончания игры */
  void (*freeGameInfo)(GameInfo_t* info) =
      nullptr; /**< Функция освобождения памяти GameInfo_t */
  bool valid = false; /**< Флаг корректной загрузки */
  QString error; /**< Сообщение об ошибке при загрузке */
};

/**
 * @brief Класс для динамической загрузки игровых библиотек и доступа к их API.
 */
class LibraryLoader : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский объект Qt
   */
  explicit LibraryLoader(QObject* parent = nullptr);

  /**
   * @brief Деструктор. Вызывает unloadGame().
   */
  ~LibraryLoader();

  /**
   * @brief Загружает библиотеку выбранной игры.
   * @param gameType Тип игры для загрузки.
   * @return true, если библиотека успешно загружена.
   */
  bool loadGame(GameType gameType);

  /**
   * @brief Выгружает текущую библиотеку и сбрасывает API.
   */
  void unloadGame();

  /**
   * @brief Возвращает API загруженной игры.
   * @return Структура GameAPI с указателями на функции игры.
   */
  GameAPI getAPI() const { return m_api; }

  /**
   * @brief Проверяет, загружена ли библиотека корректно.
   * @return true, если библиотека загружена и API валидно.
   */
  bool isLoaded() const { return m_api.valid; }

 private:
  /**
   * @brief Текущий API загруженной игры.
   */
  GameAPI m_api;

  /**
   * @brief Формирует путь к библиотеке в зависимости от типа игры.
   * @param gameType Тип игры.
   * @return Полный путь к файлу библиотеки.
   */
  QString getLibraryPath(GameType gameType) const;
};

#endif  // LIBRARYLOADER_H
