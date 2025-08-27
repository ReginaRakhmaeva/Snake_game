/**
 * @file mainwindow.cpp
 * @brief Реализация главного окна приложения BrickGame.
 *
 * Этот файл содержит реализацию класса MainWindow, который отвечает за
 * отображение игрового интерфейса, обработку пользовательского ввода,
 * обновление панели информации и взаимодействие с GameController.
 */
#include "../../../include/gui/desktop/mainwindow.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

#include "../../../include/brickgame/common/types.h"
#include "../../../include/gui/desktop/gameoverdialog.h"
#include "../../../include/gui/desktop/gameselectiondialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_gameController(new GameController(this)),
      m_gameWidget(new GameWidget(this)),
      m_currentGameType(GameType::TETRIS) {
  setupUI();
  setupConnections();

  hide();

  m_gameController->showGameSelection();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  setWindowTitle("BrickGame Collection");
  setFixedSize(800, 600);
  setStyleSheet(
      "QMainWindow { "
      "    background-color: #2c3e50; "
      "}");

  m_centralWidget = new QWidget(this);
  setCentralWidget(m_centralWidget);

  m_mainLayout = new QHBoxLayout(m_centralWidget);
  m_mainLayout->setSpacing(10);
  m_mainLayout->setContentsMargins(10, 10, 10, 10);

  setupGameWidget();

  setupInfoPanel();
}

void MainWindow::setupGameWidget() {
  m_gameWidget->setFixedSize(400, 580);
  m_gameWidget->setStyleSheet(
      "QWidget { "
      "    background-color: #34495e; "
      "    border: 2px solid #ecf0f1; "
      "    border-radius: 5px; "
      "}");
  m_mainLayout->addWidget(m_gameWidget);
}

void MainWindow::setupInfoPanel() {
  m_infoPanel = new QWidget(this);
  m_infoPanel->setFixedWidth(200);
  m_infoPanel->setStyleSheet(
      "QWidget { "
      "    background-color: #2c346fff; "
      "    border: 2px solid #3498db; "
      "    border-radius: 5px; "
      "    color: white; "
      "}");

  QVBoxLayout* infoLayout = new QVBoxLayout(m_infoPanel);
  infoLayout->setSpacing(10);
  infoLayout->setContentsMargins(10, 10, 10, 10);

  QString labelStyle =
      "QLabel { "
      "    font-size: 16px; "
      "    font-weight: bold; "
      "    color: #ecf0f1; "
      "    background-color: #2c3e50; "
      "    border-radius: 3px; "
      "    padding: 8px; "
      "}";

  QLabel* titleLabel = new QLabel("GAME INFO", m_infoPanel);
  titleLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(titleLabel);

  m_scoreLabel = new QLabel("Score: 0", m_infoPanel);
  m_scoreLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_scoreLabel);

  m_highScoreLabel = new QLabel("High Score: 0", m_infoPanel);
  m_highScoreLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_highScoreLabel);

  m_levelLabel = new QLabel("Level: 1", m_infoPanel);
  m_levelLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_levelLabel);

  m_nextLabel = new QLabel("Next:", m_infoPanel);
  m_nextLabel->setStyleSheet(labelStyle);
  m_nextLabel->setVisible(false);
  infoLayout->addWidget(m_nextLabel);

  m_nextFigureWidget = new QWidget(m_infoPanel);
  m_nextFigureWidget->setFixedSize(180, 120);
  m_nextFigureWidget->setStyleSheet(labelStyle);
  m_nextFigureWidget->setVisible(false);

  QVBoxLayout* nextFigureLayout = new QVBoxLayout(m_nextFigureWidget);
  nextFigureLayout->setContentsMargins(0, 0, 0, 0);
  nextFigureLayout->setSpacing(0);

  infoLayout->addWidget(m_nextFigureWidget);

  QString buttonStyle =
      "QPushButton { "
      "    background-color: #3498db; "
      "    border: 2px solid #2980b9; "
      "    border-radius: 5px; "
      "    color: white; "
      "    font-size: 14px; "
      "    font-weight: bold; "
      "    padding: 12px; "
      "} "
      "QPushButton:hover { "
      "    background-color: #2980b9; "
      "} "
      "QPushButton:pressed { "
      "    background-color: #21618c; "
      "}";

  m_startButton = new QPushButton("Start", m_infoPanel);
  m_startButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_startButton);

  m_pauseButton = new QPushButton("Pause", m_infoPanel);
  m_pauseButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_pauseButton);

  m_quitButton = new QPushButton("Quit", m_infoPanel);
  m_quitButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_quitButton);

  m_mainLayout->addWidget(m_infoPanel);
}

void MainWindow::setupConnections() {
  connect(m_gameController, &GameController::gameStateChanged, this,
          &MainWindow::onGameStateChanged);
  connect(m_gameController, &GameController::gameOver, this,
          &MainWindow::onGameOver);
  connect(m_gameController, &GameController::gameWon, this,
          &MainWindow::onGameWon);
  connect(m_gameController, &GameController::gamePaused, this,
          &MainWindow::onGamePaused);
  connect(m_gameController, &GameController::gameResumed, this,
          &MainWindow::onGameResumed);
  connect(m_gameController, &GameController::gameSelected, this,
          &MainWindow::onGameSelected);
  connect(m_gameController, &GameController::showGameSelectionRequested, this,
          &MainWindow::onShowGameSelectionRequested);
  connect(m_gameController, &GameController::applicationCloseRequested, this,
          &MainWindow::onApplicationCloseRequested);

  connect(m_startButton, &QPushButton::clicked, this,
          &MainWindow::onStartButtonClicked);
  connect(m_pauseButton, &QPushButton::clicked, this,
          &MainWindow::onPauseButtonClicked);
  connect(m_quitButton, &QPushButton::clicked, this,
          &MainWindow::onQuitButtonClicked);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  m_gameController->handleKeyPress(event->key());
}

void MainWindow::closeEvent(QCloseEvent* event) {
  if (m_gameController->isGameStarted()) {
    m_gameController->stopGame();
  }
  m_gameController->closeApplication();
  event->accept();
}

void MainWindow::onGameStateChanged(const GameInfo_t& state) {
  m_gameWidget->updateGameState(state);
  updateInfoPanel(state);
}

void MainWindow::onGameOver() {
  GameOverDialog dialog(false, this);
  connect(&dialog, &GameOverDialog::restartRequested, m_gameController,
          &GameController::handleRestartGame);
  connect(&dialog, &GameOverDialog::quitRequested, m_gameController,
          &GameController::closeApplication);
  dialog.exec();
}

void MainWindow::onGameWon() {
  GameOverDialog dialog(true, this);
  connect(&dialog, &GameOverDialog::restartRequested, m_gameController,
          &GameController::handleRestartGame);
  connect(&dialog, &GameOverDialog::quitRequested, m_gameController,
          &GameController::closeApplication);
  dialog.exec();
}

void MainWindow::onGamePaused() {}

void MainWindow::onGameResumed() {}

void MainWindow::onGameSelected(GameType gameType) {
  m_currentGameType = gameType;
  show();

  m_gameWidget->setGameType(gameType);
  m_gameWidget->showStartScreen();

  updateUIForGameType(gameType);
}

void MainWindow::onShowGameSelectionRequested() {
  GameSelectionDialog dialog(this);
  connect(&dialog, &GameSelectionDialog::gameSelected, m_gameController,
          &GameController::handleGameSelection);
  connect(&dialog, &GameSelectionDialog::dialogRejected, m_gameController,
          &GameController::closeApplication);

  dialog.exec();
}

void MainWindow::onApplicationCloseRequested() { close(); }

void MainWindow::updateInfoPanel(const GameInfo_t& state) {
  m_scoreLabel->setText(QString("Score: %1").arg(state.score));
  m_highScoreLabel->setText(QString("High Score: %1").arg(state.high_score));
  m_levelLabel->setText(QString("Level: %1").arg(state.level));

  if (m_currentGameType == GameType::TETRIS) {
    drawNextFigure(state);
  }
}

void MainWindow::drawNextFigure(const GameInfo_t& state) {
  if (!state.next) {
    return;
  }

  if (!m_nextFigureWidget) {
    return;
  }

  QPixmap pixmap(180, 120);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);

  int cellSize = 30;
  int figureWidth = 4 * cellSize;
  int figureHeight = 4 * cellSize;

  int offsetX = (180 - figureWidth) / 2;
  int offsetY = (120 - figureHeight) / 2;

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      if (state.next[y] && state.next[y][x]) {
        QRect cellRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize,
                       cellSize);
        painter.fillRect(cellRect, QColor(0, 0, 255));
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cellRect);
      }
    }
  }

  QLabel* nextFigureLabel = nullptr;
  QVBoxLayout* layout =
      qobject_cast<QVBoxLayout*>(m_nextFigureWidget->layout());

  if (layout && layout->count() > 0) {
    nextFigureLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
  }

  if (!nextFigureLabel) {
    nextFigureLabel = new QLabel();
    nextFigureLabel->setAlignment(Qt::AlignCenter);
    nextFigureLabel->setFixedSize(180, 120);
    if (layout) {
      layout->addWidget(nextFigureLabel);
    }
  }

  nextFigureLabel->setPixmap(pixmap);
  nextFigureLabel->show();
  m_nextFigureWidget->update();
}

void MainWindow::onStartButtonClicked() {
  m_gameController->handleStartButton();
}

void MainWindow::onPauseButtonClicked() {
  m_gameController->handlePauseButton();
}

void MainWindow::onQuitButtonClicked() { m_gameController->handleQuitButton(); }

void MainWindow::showStartScreen() { m_gameWidget->showStartScreen(); }

void MainWindow::showGameOverScreen() { m_gameWidget->showGameOverScreen(); }

void MainWindow::showGameWonScreen() { m_gameWidget->showGameWonScreen(); }

void MainWindow::updateUIForGameType(GameType gameType) {
  bool isTetris = (gameType == GameType::TETRIS);
  m_nextLabel->setVisible(isTetris);
  m_nextFigureWidget->setVisible(isTetris);
}
