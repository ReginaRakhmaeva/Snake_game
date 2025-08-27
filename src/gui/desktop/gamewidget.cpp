/**
 * @file gamewidget.cpp
 * @brief Реализация виджета игрового поля для Desktop GUI.
 *
 * В этом файле реализованы методы GameWidget:
 *  - paintEvent(): перерисовывает виджет в зависимости от текущего экрана.
 *  - drawGameField(): рисует игровое поле и ячейки.
 *  - drawStartScreen(): отображает стартовую заставку и инструкции.
 *  - drawGameOverScreen() / drawGameWonScreen(): выводят финальные сообщения.
 *  - drawBorders(): прорисовывает границы и линии сетки игрового поля.
 *  - getCellColor() / getCellRect(): вычисляет цвет ячейки и координаты для
 * рисования.
 *
 * Виджет использует QPainter для отрисовки, QFont для текста и QColor для
 * цветов. Игровое поле центрируется на виджете, а размер ячеек подстраивается
 * под окно.
 */

#include "../../../include/gui/desktop/gamewidget.h"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QtMath>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
      m_currentGameType(GameType::TETRIS),
      m_currentScreen(ScreenType::START) {
  setFocusPolicy(Qt::StrongFocus);
  setStyleSheet("QWidget { background-color: black; }");
}

GameWidget::~GameWidget() {}

void GameWidget::updateGameState(const GameInfo_t& state) {
  m_currentState = state;
  m_currentScreen = ScreenType::GAME;
  update();
}

void GameWidget::showStartScreen() {
  m_currentScreen = ScreenType::START;
  update();
}

void GameWidget::showGameOverScreen() {
  m_currentScreen = ScreenType::GAME_OVER;
  update();
}

void GameWidget::showGameWonScreen() {
  m_currentScreen = ScreenType::GAME_WON;
  update();
}

void GameWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), QColor(44, 62, 80));

  switch (m_currentScreen) {
    case ScreenType::START:
      drawStartScreen(painter);
      break;
    case ScreenType::GAME:
      drawGameField(painter);
      break;
    case ScreenType::GAME_OVER:
      drawGameOverScreen(painter);
      break;
    case ScreenType::GAME_WON:
      drawGameWonScreen(painter);
      break;
  }
}

void GameWidget::drawGameField(QPainter& painter) {
  if (!m_currentState.field) return;

  drawBorders(painter);

  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      if (m_currentState.field[y] && m_currentState.field[y][x]) {
        QRect cellRect = getCellRect(x, y);
        painter.fillRect(cellRect, getCellColor(m_currentState.field[y][x]));
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cellRect);
      }
    }
  }
}

void GameWidget::drawStartScreen(QPainter& painter) {
  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 28, QFont::Bold));

  QString title = "BRICKGAME";
  QFontMetrics fmTitle(painter.font());
  int titleWidth = fmTitle.horizontalAdvance(title);
  painter.drawText((width() - titleWidth) / 2, height() / 2 - 120, title);

  painter.setFont(QFont("Arial", 16, QFont::Normal));
  QFontMetrics fmInstructions(painter.font());

  QStringList instructions;
  if (m_currentGameType == GameType::TETRIS) {
    instructions = {
        "Press ENTER to Start", "or Q to Quit",  "",         "TETRIS CONTROLS",
        "Arrow Keys: Move",     "Space: Rotate", "P: Pause", "Q: Quit"};
  } else {
    instructions = {"Press ENTER to Start",
                    "or Q to Quit",
                    "",
                    "SNAKE CONTROLS",
                    "Arrow Keys: Move",
                    "3 same moves: Speed up",
                    "P: Pause",
                    "Q: Quit"};
  }

  int y = height() / 2 - 40;
  for (const QString& instruction : instructions) {
    if (!instruction.isEmpty()) {
      int textWidth = fmInstructions.horizontalAdvance(instruction);
      painter.drawText((width() - textWidth) / 2, y, instruction);
    }
    y += 30;
  }
}

void GameWidget::drawGameOverScreen(QPainter& painter) {
  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 28, QFont::Bold));

  QString title = "GAME OVER";
  QFontMetrics fm(painter.font());
  int titleWidth = fm.horizontalAdvance(title);
  painter.drawText((width() - titleWidth) / 2, height() / 2 - 60, title);

  painter.setFont(QFont("Arial", 16, QFont::Normal));
  QStringList messages = {"Press ENTER to restart", "Press Q to exit"};

  int y = height() / 2;
  for (const QString& message : messages) {
    int textWidth = fm.horizontalAdvance(message);
    painter.drawText((width() - textWidth) / 2, y, message);
    y += 30;
  }
}

void GameWidget::drawGameWonScreen(QPainter& painter) {
  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 28, QFont::Bold));

  QString title = "YOU WON!";
  QFontMetrics fm(painter.font());
  int titleWidth = fm.horizontalAdvance(title);
  painter.drawText((width() - titleWidth) / 2, height() / 2 - 60, title);

  painter.setFont(QFont("Arial", 16, QFont::Normal));
  QStringList messages = {"Press ENTER to restart", "Press Q to exit"};

  int y = height() / 2;
  for (const QString& message : messages) {
    int textWidth = fm.horizontalAdvance(message);
    painter.drawText((width() - textWidth) / 2, y, message);
    y += 30;
  }
}

void GameWidget::drawBorders(QPainter& painter) {
  int cellSize = qMin(width() / kGameWidth, height() / kGameHeight);

  int offsetX = (width() - kGameWidth * cellSize) / 2;
  int offsetY = (height() - kGameHeight * cellSize) / 2;

  painter.setPen(QPen(Qt::white, BORDER_WIDTH));

  QRect borderRect(offsetX, offsetY, kGameWidth * cellSize,
                   kGameHeight * cellSize);
  painter.drawRect(borderRect);

  painter.setPen(QPen(Qt::gray, 1));

  for (int x = 1; x < kGameWidth; ++x) {
    painter.drawLine(offsetX + x * cellSize, offsetY, offsetX + x * cellSize,
                     offsetY + kGameHeight * cellSize);
  }
  for (int y = 1; y < kGameHeight; ++y) {
    painter.drawLine(offsetX, offsetY + y * cellSize,
                     offsetX + kGameWidth * cellSize, offsetY + y * cellSize);
  }
}

QColor GameWidget::getCellColor(int cellValue) const {
  switch (cellValue) {
    case 1:
      return QColor(0, 255, 0);
    case 2:
      return QColor(255, 0, 0);
    default:
      return QColor(128, 128, 128);
  }
}

QRect GameWidget::getCellRect(int x, int y) const {
  int cellSize = qMin(width() / kGameWidth, height() / kGameHeight);

  int offsetX = (width() - kGameWidth * cellSize) / 2;
  int offsetY = (height() - kGameHeight * cellSize) / 2;

  return QRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize,
               cellSize);
}

void GameWidget::setGameType(GameType gameType) {
  m_currentGameType = gameType;
}
