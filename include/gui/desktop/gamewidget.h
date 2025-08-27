/**
 * @file gamewidget.h
 * @brief Заголовочный файл виджета игрового поля для Desktop GUI.
 *
 * Класс GameWidget наследует QWidget и отвечает за отображение текущего
 * состояния игры. Поддерживает разные экраны: стартовый, игровой, "Game Over" и
 * "You Won". Отвечает за прорисовку игрового поля, границ, ячеек, сообщений и
 * инструкций.
 *
 * Основные возможности:
 *  - Обновление состояния игры через updateGameState().
 *  - Показ стартового, финального и победного экранов.
 *  - Настройка типа игры (Tetris или Snake).
 *  - Отрисовка игрового поля с цветными ячейками.
 */

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QKeyEvent>
#include <QPainter>
#include <QWidget>

#include "../../brickgame/common/game_constants.h"
#include "../../brickgame/common/types.h"
#include "libraryloader.h"

class GameWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор. Инициализирует виджет и устанавливает фокус.
   */
  explicit GameWidget(QWidget* parent = nullptr);

  /**
   * @brief Деструктор. Освобождает ресурсы виджета.
   */
  ~GameWidget();

  /**
   * @brief Обновляет состояние игры и отображает игровое поле.
   * @param state Текущее состояние игры (GameInfo_t).
   */
  void updateGameState(const GameInfo_t& state);

  /**
   * @brief Показывает стартовый экран игры.
   */
  void showStartScreen();

  /**
   * @brief Показывает экран "Game Over".
   */
  void showGameOverScreen();

  /**
   * @brief Показывает экран победы ("You Won").
   */
  void showGameWonScreen();

  /**
   * @brief Устанавливает текущий тип игры (Tetris или Snake).
   */
  void setGameType(GameType gameType);

 protected:
  /**
   * @brief Переопределение метода QWidget для перерисовки виджета.
   * Вызывает соответствующие функции рисования в зависимости от текущего
   * экрана.
   * @param event Событие перерисовки.
   */
  void paintEvent(QPaintEvent* event) override;

 private:
  /**
   * @brief Отрисовывает игровое поле с текущим состоянием игры.
   * Рисует все ячейки и сетку.
   * @param painter Объект QPainter для рисования.
   */
  void drawGameField(QPainter& painter);

  /**
   * @brief Отрисовывает стартовый экран с названием игры и инструкциями.
   * @param painter Объект QPainter для рисования.
   */
  void drawStartScreen(QPainter& painter);

  /**
   * @brief Отрисовывает экран "Game Over" с сообщением и инструкциями.
   * @param painter Объект QPainter для рисования.
   */
  void drawGameOverScreen(QPainter& painter);

  /**
   * @brief Отрисовывает экран победы ("You Won") с сообщением и инструкциями.
   * @param painter Объект QPainter для рисования.
   */
  void drawGameWonScreen(QPainter& painter);

  /**
   * @brief Рисует границы игрового поля и сетку ячеек.
   * @param painter Объект QPainter для рисования.
   */
  void drawBorders(QPainter& painter);

  /**
   * @brief Возвращает цвет ячейки по её значению.
   * @param cellValue Значение ячейки в поле.
   * @return Цвет ячейки.
   */
  QColor getCellColor(int cellValue) const;

  /**
   * @brief Вычисляет прямоугольник для ячейки на экране.
   * @param x Координата X ячейки в массиве поля.
   * @param y Координата Y ячейки в массиве поля.
   * @return QRect для рисования ячейки.
   */
  QRect getCellRect(int x, int y) const;

  /** @brief Текущее состояние игры для отрисовки. */
  GameInfo_t m_currentState;

  /** @brief Текущий выбранный тип игры (Tetris или Snake). */
  GameType m_currentGameType;

  /** @brief Текущий отображаемый экран виджета. */
  enum class ScreenType { START, GAME, GAME_OVER, GAME_WON } m_currentScreen;

  /** @brief Размер одной ячейки игрового поля в пикселях. */
  static const int CELL_SIZE = 25;

  /** @brief Толщина границы игрового поля в пикселях. */
  static const int BORDER_WIDTH = 2;
};

#endif  // GAMEWIDGET_H
