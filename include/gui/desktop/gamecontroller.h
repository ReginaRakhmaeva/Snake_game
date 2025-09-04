/**
 * @file gamecontroller.h
 * @brief Тонкий контроллер игры для GUI на Qt.
 *
 * Класс GameController координирует работу:
 * - InputHandler - обработка ввода
 * - GameStateManager - управление состоянием
 * - TimerManager - управление таймером
 * - LibraryLoader - загрузка игр
 *
 * Тонкий контроллер только координирует, не содержит бизнес-логику.
 */
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <memory>

#include "../../brickgame/common/types.h"
#include "inputhandler.h"
#include "libraryloader.h"
#include "timermanager.h"

/**
 * @brief Тонкий контроллер игры.
 *
 * Координирует работу специализированных компонентов:
 * - InputHandler для обработки ввода
 * - TimerManager для управления таймером
 * - LibraryLoader для загрузки игр
 *
 * Состояние игры получается только из backend FSM через GameInfo_t
 */
class GameController : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский объект Qt
   */
  explicit GameController(QObject* parent = nullptr);

  /**
   * @brief Деструктор.
   */
  ~GameController();

  /**
   * @brief Загружает игру заданного типа.
   * @param gameType Тип игры
   * @return true если успешно загружена
   */
  bool loadGame(GameType gameType);

  /**
   * @brief Выгружает текущую игру.
   */
  void unloadGame();

  /**
   * @brief Обрабатывает нажатие клавиши.
   * @param key Код клавиши Qt
   */
  void handleKeyPress(int key);

  /**
   * @brief Обрабатывает отпускание клавиши.
   * @param key Код клавиши Qt
   */
  void handleKeyRelease(int key);

  /**
   * @brief Обрабатывает действие от InputHandler.
   * @param action Действие пользователя
   */
  void handleAction(UserAction_t action);

  /**
   * @brief Обрабатывает отпускание клавиши от InputHandler.
   * @param key Код клавиши Qt
   */
  void handleKeyReleased(int key);

  /**
   * @brief Получает текущее состояние игры.
   * @return Состояние игры
   */
  GameInfo_t getCurrentState() const;

  /**
   * @brief Проверяет, завершена ли игра.
   * @return true если игра завершена
   */
  bool isGameOver() const;

  /**
   * @brief Проверяет, запущена ли игра.
   * @return true если игра запущена (не на паузе и не завершена)
   */
  bool isGameStarted() const;

  /**
   * @brief Проверяет, находится ли игра на паузе.
   * @return true если игра на паузе
   */
  bool isGamePaused() const;

  /**
   * @brief Получает текущий тип игры.
   * @return Тип игры
   */
  GameType getCurrentGameType() const;

  // Методы для совместимости с UI
  void showGameSelection();
  void stopGame();
  void closeApplication();
  void handleRestartGame();
  void handleGameSelection(GameType gameType);
  void handleStartButton();
  void handlePauseButton();
  void handleQuitButton();

 signals:
  void gameStateChanged(
      const GameInfo_t& state); /**< Изменение состояния игры */
  void gameOver();              /**< Завершение игры */
  void gameWon();               /**< Победа */
  void gamePaused();            /**< Пауза */
  void gameResumed();           /**< Возобновление */
  void gameSelected(GameType gameType); /**< Выбор игры */
  void showGameSelectionRequested(); /**< Запрос меню выбора */
  void applicationCloseRequested();  /**< Запрос закрытия */

 private slots:
  /**
   * @brief Обновляет состояние игры по таймеру.
   */
  void updateGame();

 protected:
  std::unique_ptr<LibraryLoader> m_libraryLoader; /**< Загрузчик библиотек */
  std::unique_ptr<InputHandler> m_inputHandler; /**< Обработчик ввода */
  std::unique_ptr<TimerManager> m_timerManager; /**< Менеджер таймера */
  GameType m_currentGameType; /**< Текущий тип игры */
  bool m_wasPaused; /**< Предыдущее состояние паузы */

 private:
  /**
   * @brief Инициализирует соединения сигналов.
   */
  void setupConnections();

  /**
   * @brief Обновляет состояние игры в библиотеке.
   */
  void updateGameState();
};

#endif  // GAMECONTROLLER_H
