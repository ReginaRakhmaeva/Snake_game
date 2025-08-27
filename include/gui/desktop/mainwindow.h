/**
 * @file mainwindow.h
 * @brief Главное окно приложения BrickGame. Содержит игровую область, панель
 * информации и кнопки управления.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "gamecontroller.h"
#include "gamewidget.h"

/**
 * @brief Класс главного окна приложения BrickGame.
 * Отвечает за отображение интерфейса, взаимодействие с пользователем и
 * синхронизацию с GameController.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский виджет (по умолчанию nullptr)
   */
  explicit MainWindow(QWidget* parent = nullptr);

  /**
   * @brief Деструктор.
   */
  ~MainWindow();

 protected:
  /**
   * @brief Обработчик нажатий клавиш.
   * @param event Событие нажатия клавиши
   */
  void keyPressEvent(QKeyEvent* event) override;

  /**
   * @brief Обработчик закрытия окна.
   * @param event Событие закрытия
   */
  void closeEvent(QCloseEvent* event) override;

 private slots:
  /**
   * @brief Слот для обновления состояния игры.
   */
  void onGameStateChanged(const GameInfo_t& state);

  /**
   * @brief Слот вызова диалога GameOver.
   */
  void onGameOver();

  /**
   * @brief Слот вызова диалога GameWon.
   */
  void onGameWon();

  /** @brief Слот обработки паузы игры */
  void onGamePaused();

  /** @brief Слот возобновления игры */
  void onGameResumed();

  /**
   * @brief Слот при выборе игры пользователем.
   */
  void onGameSelected(GameType gameType);

  /**
   * @brief Показ диалога выбора игры.
   */
  void onShowGameSelectionRequested();

  /**
   * @brief Слот для закрытия приложения.
   * Вызывается, когда GameController запрашивает завершение работы.
   * Закрывает главное окно и завершает приложение.
   */
  void onApplicationCloseRequested();

  /**
   * @brief Слоты обработки кнопок Start
   */
  void onStartButtonClicked();

  /**
   * @brief Слоты обработки кнопок  Pause
   */
  void onPauseButtonClicked();

  /**
   * @brief Слоты обработки кнопок Quit.
   */
  void onQuitButtonClicked();

 private:
  /** @brief Создание и настройка интерфейса */
  void setupUI();

  /** @brief Настройка виджета игрового поля */
  void setupGameWidget();

  /** @brief Настройка панели информации */
  void setupInfoPanel();

  /** @brief Обновление информации на панели (очки, уровень, следующая фигура)
   */
  void updateInfoPanel(const GameInfo_t& state);

  /** @brief Показ стартового экрана */
  void showStartScreen();

  /** @brief Показ экрана GameOver */
  void showGameOverScreen();

  /** @brief Показ экрана победы */
  void showGameWonScreen();

  /** @brief Подключение сигналов и слотов */
  void setupConnections();

  /**
   * @brief Отрисовка следующей фигуры для Tetris.
   */
  void drawNextFigure(const GameInfo_t& state);
  /**
   * @brief Обновление UI для выбранного типа игры.
   */
  void updateUIForGameType(GameType gameType);

  GameController* m_gameController; /**< Контроллер игры */
  GameWidget* m_gameWidget;         /**< Виджет игрового поля */

  QWidget* m_infoPanel;        /**< Панель информации */
  QLabel* m_scoreLabel;        /**< Метка для отображения очков */
  QLabel* m_highScoreLabel;    /**< Метка для отображения рекорда */
  QLabel* m_levelLabel;        /**< Метка для отображения уровня */
  QLabel* m_nextLabel;         /**< Метка "Next" для Tetris */
  QWidget* m_nextFigureWidget; /**< Виджет для отображения следующей фигуры */

  QPushButton* m_startButton; /**< Кнопка Start */
  QPushButton* m_pauseButton; /**< Кнопка Pause */
  QPushButton* m_quitButton;  /**< Кнопка Quit */

  QWidget* m_centralWidget;  /**< Центральный виджет окна */
  QHBoxLayout* m_mainLayout; /**< Основной горизонтальный лейаут */

  GameType m_currentGameType; /**< Текущий выбранный тип игры */
};

#endif  // MAINWINDOW_H
