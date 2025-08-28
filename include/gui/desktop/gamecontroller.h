/**
 * @file gamecontroller.h
 * @brief Контроллер игры для GUI на Qt.
 *
 * Класс GameController управляет:
 * - загрузкой/выгрузкой выбранной игры (Tetris, Snake) через LibraryLoader,
 * - состояниями игры (старт, пауза, возобновление, остановка),
 * - обработкой пользовательского ввода и конвертацией клавиш в игровые
 * действия,
 * - синхронизацией игрового цикла через QTimer,
 * - генерацией сигналов Qt для обновления интерфейса.
 *
 */
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QKeyEvent>
#include <QObject>
#include <QTimer>
#include <memory>
#include <QSet>

#include "../../brickgame/common/types.h"
#include "libraryloader.h"
/**
 * @brief Контроллер игры для GUI на Qt.
 *
 * Управляет загрузкой игр, состояниями игрового процесса,
 * обработкой пользовательского ввода и генерацией сигналов для интерфейса.
 */
class GameController : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор контроллера игры.
   * @param parent Родительский QObject.
   */
  explicit GameController(QObject* parent = nullptr);

  /**
   * @brief Деструктор контроллера игры.
   */
  ~GameController();

  /**
   * @brief Загружает игру заданного типа через LibraryLoader.
   * @param gameType Тип игры (Tetris, Snake).
   * @return true, если игра успешно загружена, иначе false.
   */
  bool loadGame(GameType gameType);

  /**
   * @brief Выгружает текущую игру, останавливает таймер и сбрасывает состояние.
   */
  void unloadGame();

  /**
   * @brief Запускает игру. Если игра уже загружена, инициализирует состояние и
   * таймер.
   */
  void startGame();

  /**
   * @brief Ставит игру на паузу.
   */
  void pauseGame();

  /**
   * @brief Возобновляет игру после паузы.
   */
  void resumeGame();

  /**
   * @brief Останавливает игру.
   */
  void stopGame();

  /**
   * @brief Передает действие пользователя в игровую библиотеку.
   * @param action Действие пользователя.
   * @param hold Флаг удержания клавиши (по умолчанию false).
   */
  void processInput(UserAction_t action, bool hold = false);
  /**
   * @brief Получает текущее состояние игры.
   * @return GameInfo_t с информацией о поле, очках, уровне и скорости.
   */
  GameInfo_t getCurrentState() const;

  /**
   * @brief Проверяет, завершена ли игра.
   * @return true, если игра окончена, иначе false.
   */
  bool isGameOver() const;

  /**
   * @brief Проверяет, запущена ли игра.
   * @return true, если игра активна, иначе false.
   */
  bool isGameStarted() const;

  /**
   * @brief Проверяет, находится ли игра на паузе.
   * @return true, если игра на паузе, иначе false.
   */
  bool isGamePaused() const;

  /**
   * @brief Получает текущий тип загруженной игры.
   * @return GameType (Tetris или Snake).
   */
  GameType getCurrentGameType() const;

  /**
   * @brief Преобразует код клавиши Qt в действие игры.
   * @param key Код клавиши Qt.
   * @return UserAction_t соответствующее действие или -1, если клавиша не
   * распознана.
   */
  UserAction_t mapKeyToAction(int key) const;

  /**
   * @brief Обрабатывает нажатие клавиши пользователем.
   * @param key Код клавиши Qt.
   */
  void handleKeyPress(int key);

  /**
   * @brief Обрабатывает отпускание клавиши пользователем.
   * @param key Код клавиши Qt.
   */
  void handleKeyRelease(int key);

  /**
   * @brief Обрабатывает нажатие кнопки "Старт" в интерфейсе.
   */
  void handleStartButton();

  /**
   * @brief Обрабатывает нажатие кнопки "Пауза/Возобновить" в интерфейсе.
   */
  void handlePauseButton();

  /**
   * @brief Обрабатывает нажатие кнопки "Выход" в интерфейсе.
   */
  void handleQuitButton();

  /**
   * @brief Выбирает игру для запуска.
   * @param gameType Тип игры.
   */
  void handleGameSelection(GameType gameType);

  /**
   * @brief Перезапускает текущую игру.
   */
  void handleRestartGame();

  /**
   * @brief Запрашивает показ меню выбора игры.
   */
  void showGameSelection();

  /**
   * @brief Запрашивает закрытие приложения.
   */
  void closeApplication();

 signals:
  void gameStateChanged(
      const GameInfo_t& state);         /**< Сигнал изменения состояния игры */
  void gameOver();                      /**< Сигнал завершения игры */
  void gameWon();                       /**< Сигнал победы */
  void gamePaused();                    /**< Сигнал постановки на паузу */
  void gameResumed();                   /**< Сигнал возобновления игры */
  void gameSelected(GameType gameType); /**< Сигнал выбора игры */
  void showGameSelectionRequested();    /**< Сигнал запроса меню выбора игры */
  void applicationCloseRequested(); /**< Сигнал запроса закрытия приложения */

 private slots:
  /**
   * @brief Слот, вызываемый таймером QTimer, обновляет состояние игры.
   */
  void updateGame();

 private:
  LibraryLoader* m_libraryLoader; /**< Загрузчик динамических библиотек игры */
  GameType m_currentGameType;     /**< Текущий тип игры */
  QTimer* m_gameTimer;            /**< Таймер для игрового цикла */
  bool m_gameStarted; /**< Флаг состояния игры: запущена/не запущена */
  bool m_gamePaused;  /**< Флаг состояния игры: на паузе/не на паузе */
  QSet<int> m_pressedKeys; /**< Множество нажатых клавиш для отслеживания удержания */
};

#endif  // GAMECONTROLLER_H
