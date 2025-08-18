
#include "../../../include/gui/desktop/mainwindow.h"

#include <QAction>
#include <QMenuBar>

#include "../../../include/gui/desktop/gamewidget.h"

extern "C" {
#include "../../../include/brickgame/snake/snake_api.h"
#include "../../../include/brickgame/tetris/game.h"
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  gameWidget = new GameWidget(this);
  setCentralWidget(gameWidget);
  setWindowTitle("BrickGame Collection");
  resize(400, 800);

  QMenu* gameMenu = menuBar()->addMenu("Game");
  QAction* snakeAction = new QAction("Snake", this);
  QAction* tetrisAction = new QAction("Tetris", this);
  gameMenu->addAction(snakeAction);
  gameMenu->addAction(tetrisAction);

  connect(snakeAction, &QAction::triggered, this, &MainWindow::setSnake);
  connect(tetrisAction, &QAction::triggered, this, &MainWindow::setTetris);

  setSnake();  // По умолчанию запускаем змейку
}

MainWindow::~MainWindow() {}

void MainWindow::setSnake() {
  gameWidget->setGameAPI(&userInput, &updateCurrentState, &isGameOver);
}

void MainWindow::setTetris() {
  gameWidget->setGameAPI(&userInput,           // из tetris/game.h!
                         &updateCurrentState,  // из tetris/game.h!
                         &isGameOver           // из tetris/game.h!
  );
}