#include "../../../include/gui/desktop/gamewidget.h"

#include <QKeyEvent>
#include <QPainter>

GameWidget::GameWidget(QWidget* parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWidget::gameTick);
  timer->start(100);
}

void GameWidget::setGameAPI(UserInputFunc userInput,
                            UpdateStateFunc updateState,
                            IsGameOverFunc isGameOver) {
  userInputFunc = userInput;
  updateStateFunc = updateState;
  isGameOverFunc = isGameOver;
  started = false;
  paused = false;
  if (userInputFunc) userInputFunc(Start, false);
  if (updateStateFunc) gameInfo = updateStateFunc();
  update();
}

void GameWidget::gameTick() {
  if (!started) {
    update();
    return;
  }
  if (!paused && isGameOverFunc && !isGameOverFunc()) {
    updateGame();
  }
  update();
}

void GameWidget::sendUserInput(UserAction_t action, bool hold) {
  if (userInputFunc) userInputFunc(action, hold);
  if (action == Start) started = true;
  if (action == Pause) paused = !paused;
  if (action == Terminate) started = false;
}

void GameWidget::updateGame() {
  if (updateStateFunc) {
    gameInfo = updateStateFunc();
    timer->setInterval(gameInfo.speed);
  }
}

void GameWidget::paintEvent(QPaintEvent*) {
  QPainter p(this);
  if (!started) {
    renderStartScreen(p);
    return;
  }
  if (isGameOverFunc && isGameOverFunc()) {
    renderGameOverScreen(p);
    return;
  }
  int cellSize = qMin(width() / 10, height() / 20);
  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 10; ++x) {
      if (gameInfo.field && gameInfo.field[y][x]) {
        p.fillRect(x * cellSize, y * cellSize, cellSize, cellSize, Qt::green);
      } else {
        p.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
      }
    }
  }
  p.drawText(10, height() - 40, QString("Score: %1").arg(gameInfo.score));
  p.drawText(10, height() - 25, QString("Level: %1").arg(gameInfo.level));
  p.drawText(10, height() - 10,
             QString("High Score: %1").arg(gameInfo.high_score));
}

void GameWidget::renderStartScreen(QPainter& p) {
  p.fillRect(rect(), Qt::black);
  p.setPen(Qt::white);
  p.drawText(rect(), Qt::AlignCenter, "Press Enter to Start");
}

void GameWidget::renderGameOverScreen(QPainter& p) {
  p.fillRect(rect(), Qt::black);
  p.setPen(Qt::red);
  p.drawText(rect(), Qt::AlignCenter, "Game Over!\nPress Enter to Restart");
}

void GameWidget::keyPressEvent(QKeyEvent* e) {
  switch (e->key()) {
    case Qt::Key_Left:
      sendUserInput(Left);
      break;
    case Qt::Key_Right:
      sendUserInput(Right);
      break;
    case Qt::Key_Up:
      sendUserInput(Up);
      break;
    case Qt::Key_Down:
      sendUserInput(Down);
      break;
    case Qt::Key_Space:
      sendUserInput(Action);
      break;
    case Qt::Key_P:
      sendUserInput(Pause);
      break;
    case Qt::Key_Return:
      sendUserInput(Start);
      break;
    case Qt::Key_Escape:
      sendUserInput(Terminate);
      break;
  }
}