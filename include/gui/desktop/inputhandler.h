/**
 * @file inputhandler.h
 * @brief Обработчик пользовательского ввода для игр.
 *
 * Класс InputHandler отвечает за:
 * - Преобразование Qt клавиш в игровые действия
 * - Дебаунсинг клавиш для Tetris
 * - Отслеживание удержания клавиш для Snake
 * - Валидацию ввода
 */
#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QSet>

#include "../../brickgame/common/types.h"
#include "libraryloader.h"

/**
 * @brief Обработчик пользовательского ввода.
 */
class InputHandler : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский объект Qt
   */
  explicit InputHandler(QObject* parent = nullptr);

  /**
   * @brief Устанавливает тип игры для настройки обработки ввода.
   * @param gameType Тип игры
   */
  void setGameType(GameType gameType);

  /**
   * @brief Обрабатывает нажатие клавиши.
   * @param key Код клавиши Qt
   * @param libraryLoader Загрузчик библиотеки для передачи команд
   */
  void handleKeyPress(int key, LibraryLoader* libraryLoader);

  /**
   * @brief Обрабатывает отпускание клавиши.
   * @param key Код клавиши Qt
   */
  void handleKeyRelease(int key);

  /**
   * @brief Очищает состояние обработчика ввода.
   */
  void clear();

  /**
   * @brief Преобразует код клавиши Qt в игровое действие.
   * @param key Код клавиши Qt
   * @return Игровое действие или -1 если не распознано
   */
  UserAction_t mapKeyToAction(int key) const;

 signals:
  /**
   * @brief Сигнал для действий, требующих обработки в GameController.
   */
  void actionRequested(UserAction_t action);

  /**
   * @brief Сигнал отпускания клавиши (для Snake ускорения).
   */
  void keyReleased(int key);

 private:
  /**
   * @brief Проверяет дебаунсинг для Tetris.
   * @param key Код клавиши
   * @return true если клавиша должна быть обработана
   */
  bool checkDebounce(int key);

  GameType m_currentGameType; /**< Текущий тип игры */
  QSet<int> m_pressedKeys;    /**< Нажатые клавиши */
  QMap<int, qint64> m_lastKeyPressTime; /**< Время последнего нажатия */
  static const qint64 KEY_DEBOUNCE_MS = 100; /**< Задержка дебаунсинга */
};

#endif  // INPUTHANDLER_H
