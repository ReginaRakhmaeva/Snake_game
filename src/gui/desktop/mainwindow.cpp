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

  // Скрываем главное окно до выбора игры
  hide();

  // Показываем экран выбора игры при запуске
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

  // Центральный виджет
  m_centralWidget = new QWidget(this);
  setCentralWidget(m_centralWidget);

  // Главный layout
  m_mainLayout = new QHBoxLayout(m_centralWidget);
  m_mainLayout->setSpacing(10);
  m_mainLayout->setContentsMargins(10, 10, 10, 10);

  // Настройка виджета игры
  setupGameWidget();

  // Настройка информационной панели
  setupInfoPanel();

  // НЕ показываем стартовый экран здесь - он будет показан после выбора игры
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

  // Единый стиль для всех лейблов
  QString labelStyle =
      "QLabel { "
      "    font-size: 16px; "
      "    font-weight: bold; "
      "    color: #ecf0f1; "
      "    background-color: #2c3e50; "
      "    border-radius: 3px; "
      "    padding: 8px; "
      "}";

  // Заголовок
  QLabel* titleLabel = new QLabel("GAME INFO", m_infoPanel);
  titleLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(titleLabel);

  // Счет
  m_scoreLabel = new QLabel("Score: 0", m_infoPanel);
  m_scoreLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_scoreLabel);

  // Рекорд
  m_highScoreLabel = new QLabel("High Score: 0", m_infoPanel);
  m_highScoreLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_highScoreLabel);

  // Уровень
  m_levelLabel = new QLabel("Level: 1", m_infoPanel);
  m_levelLabel->setStyleSheet(labelStyle);
  infoLayout->addWidget(m_levelLabel);

  // Следующая фигура (для тетриса)
  m_nextLabel = new QLabel("Next:", m_infoPanel);
  m_nextLabel->setStyleSheet(labelStyle);
  m_nextLabel->setVisible(false);
  infoLayout->addWidget(m_nextLabel);

  // Виджет для отображения следующей фигуры
  m_nextFigureWidget = new QWidget(m_infoPanel);
  m_nextFigureWidget->setFixedSize(180, 120);
  m_nextFigureWidget->setStyleSheet(labelStyle);
  m_nextFigureWidget->setVisible(false);

  // Добавляем layout для правильного отображения QLabel
  QVBoxLayout* nextFigureLayout = new QVBoxLayout(m_nextFigureWidget);
  nextFigureLayout->setContentsMargins(0, 0, 0, 0);
  nextFigureLayout->setSpacing(0);

  infoLayout->addWidget(m_nextFigureWidget);

  // Единый стиль для всех кнопок
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

  // Кнопка старт (зеленая)
  m_startButton = new QPushButton("Start", m_infoPanel);
  m_startButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_startButton);

  // Кнопка пауза (синяя)
  m_pauseButton = new QPushButton("Pause", m_infoPanel);
  m_pauseButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_pauseButton);

  // Кнопка выход (красная)
  m_quitButton = new QPushButton("Quit", m_infoPanel);
  m_quitButton->setStyleSheet(buttonStyle);
  infoLayout->addWidget(m_quitButton);

  m_mainLayout->addWidget(m_infoPanel);
}

void MainWindow::setupConnections() {
  // Подключение сигналов контроллера
  connect(m_gameController, &GameController::gameStateChanged, this,
          &MainWindow::onGameStateChanged);
  connect(m_gameController, &GameController::gameOver, this,
          &MainWindow::onGameOver);
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

  // Подключение сигналов кнопок (только UI логика)
  connect(m_startButton, &QPushButton::clicked, this,
          &MainWindow::onStartButtonClicked);
  connect(m_pauseButton, &QPushButton::clicked, this,
          &MainWindow::onPauseButtonClicked);
  connect(m_quitButton, &QPushButton::clicked, this,
          &MainWindow::onQuitButtonClicked);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
      m_gameController->handleStartButton();
      break;

    case Qt::Key_P:
      m_gameController->handlePauseButton();
      break;

    case Qt::Key_Q:
      m_gameController->handleQuitButton();
      break;

    default:
      // Используем методы GameController для проверки состояния
      if (m_gameController->isGameStarted() && !m_gameController->isGamePaused()) {
        UserAction_t action = m_gameController->mapKeyToAction(event->key());
        if (action != static_cast<UserAction_t>(-1)) {
          m_gameController->processInput(action, false);
        }
      }
      break;
  }
}

void MainWindow::closeEvent(QCloseEvent* event) {
  m_gameController->stopGame();
  event->accept();
}

void MainWindow::onGameStateChanged(const GameInfo_t& state) {
  m_gameWidget->updateGameState(state);
  updateInfoPanel(state);
}

void MainWindow::onGameOver() {
  // Состояние игры управляется GameController, не нужно обновлять локальные переменные

  // Показываем диалог окончания игры
  GameOverDialog dialog(false, this);
  connect(&dialog, &GameOverDialog::restartRequested, m_gameController,
          &GameController::restartGame);
  connect(&dialog, &GameOverDialog::quitRequested, m_gameController,
          &GameController::closeApplication);
  dialog.exec();
}

void MainWindow::onGameWon() {
  // Состояние игры управляется GameController, не нужно обновлять локальные переменные

  // Показываем диалог победы
  GameOverDialog dialog(true, this);
  connect(&dialog, &GameOverDialog::restartRequested, m_gameController,
          &GameController::restartGame);
  connect(&dialog, &GameOverDialog::quitRequested, m_gameController,
          &GameController::closeApplication);
  dialog.exec();
}

void MainWindow::onGamePaused() { 
  // Состояние паузы управляется GameController
}

void MainWindow::onGameResumed() { 
  // Состояние паузы управляется GameController
}

void MainWindow::onGameSelected(GameType gameType) {
  m_currentGameType = gameType;
  
  // Показываем главное окно после выбора игры
  show();
  
  m_gameWidget->showStartScreen();

  // Показываем/скрываем next label и виджет в зависимости от игры
  bool isTetris = (gameType == GameType::TETRIS);
  m_nextLabel->setVisible(isTetris);
  m_nextFigureWidget->setVisible(isTetris);
}

void MainWindow::onShowGameSelectionRequested() {
  GameSelectionDialog dialog(this);
  connect(&dialog, &GameSelectionDialog::gameSelected, m_gameController,
          &GameController::selectGame);
  
  // Если диалог закрыт без выбора игры (через крестик), закрываем приложение
  if (dialog.exec() != QDialog::Accepted) {
    m_gameController->closeApplication();
  }
}

void MainWindow::onApplicationCloseRequested() {
  close();
}

void MainWindow::updateInfoPanel(const GameInfo_t& state) {
  m_scoreLabel->setText(QString("Score: %1").arg(state.score));
  m_highScoreLabel->setText(QString("High Score: %1").arg(state.high_score));
  m_levelLabel->setText(QString("Level: %1").arg(state.level));

  // Обновляем отображение следующей фигуры для тетриса
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

  // Создаем QPixmap для отрисовки следующей фигуры
  QPixmap pixmap(180, 120);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);

  // Размер ячейки для фигуры 4x4
  int cellSize = 30;  // Фиксированный размер ячейки
  int figureWidth = 4 * cellSize;
  int figureHeight = 4 * cellSize;

  // Центрируем фигуру в виджете
  int offsetX = (180 - figureWidth) / 2;
  int offsetY = (120 - figureHeight) / 2;

  // Рисуем следующую фигуру
  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      if (state.next[y] && state.next[y][x]) {
        QRect cellRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize,
                       cellSize);
        painter.fillRect(cellRect,
                         QColor(0, 0, 255));  // Синий цвет для тетриса
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cellRect);
      }
    }
  }

  // Получаем или создаем QLabel для отображения фигуры
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

void MainWindow::onQuitButtonClicked() {
  m_gameController->handleQuitButton();
}

void MainWindow::showStartScreen() { m_gameWidget->showStartScreen(); }

void MainWindow::showGameOverScreen() { m_gameWidget->showGameOverScreen(); }

void MainWindow::showGameWonScreen() { m_gameWidget->showGameWonScreen(); }
