/**
 * @file gamecontroller.cpp
 * @brief Реализация контроллера игры для графического интерфейса (Qt).
 *
 * Класс GameController управляет загрузкой динамических библиотек игр (Tetris,
 * Snake), обработкой пользовательского ввода, управлением состояниями игры
 * (старт, пауза, возобновление, остановка) и синхронизацией игрового цикла с
 * помощью QTimer.
 *
 * Основные возможности:
 * - Загрузка и выгрузка выбранной игры через LibraryLoader.
 * - Запуск, пауза, продолжение и остановка игры.
 * - Синхронизация логики игры через updateGame() по таймеру.
 * - Преобразование нажатий клавиш в игровые действия (mapKeyToAction).
 * - Генерация Qt-сигналов для обновления интерфейса (gameStateChanged,
 * gameOver, gamePaused и др.).
 *
 * Использует GameAPI (userInput, updateState, isOver) из подключаемых библиотек
 * игр.
 */

#include "../../../include/gui/desktop/gamecontroller.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

GameController::GameController(QObject *parent)
    : QObject(parent),
      m_libraryLoader(new LibraryLoader(this)),
      m_currentGameType(GameType::TETRIS),
      m_gameTimer(new QTimer(this)),
      m_gameStarted(false),
      m_gamePaused(false) {
  connect(m_gameTimer, &QTimer::timeout, this, &GameController::updateGame);
}

GameController::~GameController() { unloadGame(); }

bool GameController::loadGame(GameType gameType) {
  unloadGame();
  m_currentGameType = gameType;
  return m_libraryLoader->loadGame(gameType);
}

void GameController::unloadGame() {
  m_libraryLoader->unloadGame();
  m_gameStarted = false;
  m_gamePaused = false;
  m_gameTimer->stop();
}

void GameController::startGame() {
  if (!m_libraryLoader->isLoaded()) return;

  try {
    m_gameStarted = true;
    m_gamePaused = false;

    GameAPI api = m_libraryLoader->getAPI();
    api.userInput(Start, false);

    m_gameTimer->start(600);

    GameInfo_t initialState = api.updateState();
    if (initialState.field) {
      emit gameStateChanged(initialState);
    }
  } catch (...) {
    qDebug() << "Exception in startGame()";
    m_gameStarted = false;
  }
}

void GameController::pauseGame() {
  if (!m_gameStarted || m_gamePaused) return;

  m_gamePaused = true;
  m_gameTimer->stop();
  m_pressedKeys.clear();  
  emit gamePaused();
}

void GameController::resumeGame() {
  if (!m_gameStarted || !m_gamePaused) return;

  m_gamePaused = false;
  m_gameTimer->start(600);
  m_pressedKeys.clear();  
  emit gameResumed();
}

void GameController::stopGame() {
  m_gameStarted = false;
  m_gamePaused = false;
  m_gameTimer->stop();
  m_pressedKeys.clear();  
}

void GameController::processInput(UserAction_t action, bool hold) {
  if (!m_libraryLoader->isLoaded()) return;

  GameAPI api = m_libraryLoader->getAPI();
  api.userInput(action, hold);
}

GameInfo_t GameController::getCurrentState() const {
  if (!m_libraryLoader->isLoaded()) {
    GameInfo_t empty = {0};
    return empty;
  }
  GameAPI api = m_libraryLoader->getAPI();
  return api.updateState();
}

bool GameController::isGameOver() const {
  if (!m_libraryLoader->isLoaded()) return false;
  GameAPI api = m_libraryLoader->getAPI();
  return api.isOver();
}

bool GameController::isGameStarted() const { return m_gameStarted; }

bool GameController::isGamePaused() const { return m_gamePaused; }

GameType GameController::getCurrentGameType() const {
  return m_currentGameType;
}

void GameController::updateGame() {
  if (!m_libraryLoader->isLoaded() || !m_gameStarted || m_gamePaused) return;

  try {
    GameAPI api = m_libraryLoader->getAPI();
    GameInfo_t currentState = api.updateState();

    if (!currentState.field) {
      qDebug() << "Warning: currentState.field is null";
      return;
    }

    if (currentState.speed != m_gameTimer->interval()) {
      m_gameTimer->setInterval(currentState.speed);
    }

    emit gameStateChanged(currentState);

    if (api.isOver()) {
      m_gameTimer->stop();
      m_gameStarted = false;
      emit gameOver();
    }
  } catch (...) {
    qDebug() << "Exception in updateGame()";
    m_gameTimer->stop();
    m_gameStarted = false;
  }
}

UserAction_t GameController::mapKeyToAction(int key) const {
  switch (key) {
    case Qt::Key_Left:
      return Left;
    case Qt::Key_Right:
      return Right;
    case Qt::Key_Up:
      return Up;
    case Qt::Key_Down:
      return Down;
    case Qt::Key_Space:
      return Action;
    case Qt::Key_P:
      return Pause;
    case Qt::Key_Q:
      return Terminate;
    case Qt::Key_Return:
    case Qt::Key_Enter:
      return Start;
    default:
      return static_cast<UserAction_t>(-1);
  }
}

void GameController::handleKeyPress(int key) {
  
  m_pressedKeys.insert(key);
  
  switch (key) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
      handleStartButton();
      return;
    case Qt::Key_P:
      handlePauseButton();
      return;
    case Qt::Key_Q:
      handleQuitButton();
      return;
    default:
      break;
  }

  if (isGameStarted() && !isGamePaused()) {
    UserAction_t action = mapKeyToAction(key);
    if (action != static_cast<UserAction_t>(-1)) {
      
      bool hold = false;
      if (m_currentGameType == GameType::SNAKE) {
        
        hold = m_pressedKeys.contains(key);
      }
      processInput(action, hold);
    }
  }
}

void GameController::handleKeyRelease(int key) {
  
  m_pressedKeys.remove(key);
  
  
  if (m_currentGameType == GameType::SNAKE) {
    if (isGameStarted() && !isGamePaused()) {
      UserAction_t action = mapKeyToAction(key);
      if (action == Up || action == Down || action == Left || action == Right) {
        
        processInput(action, false);
      }
    }
  }
}

void GameController::handleStartButton() {
  if (!isGameStarted()) {
    startGame();
  }
}

void GameController::handlePauseButton() {
  if (isGameStarted() && !isGamePaused()) {
    pauseGame();
  } else if (isGameStarted() && isGamePaused()) {
    resumeGame();
  }
}

void GameController::handleQuitButton() {
  if (isGameStarted()) {
    stopGame();
  }
  closeApplication();
}

void GameController::handleGameSelection(GameType gameType) {
  if (loadGame(gameType)) {
    emit gameSelected(gameType);
  }
}

void GameController::handleRestartGame() {
  if (loadGame(m_currentGameType)) {
    emit gameSelected(m_currentGameType);
  }
}

void GameController::showGameSelection() { emit showGameSelectionRequested(); }

void GameController::closeApplication() { emit applicationCloseRequested(); }

GameController::GameController(const GameController& other)
    : QObject(other.parent()),
      m_libraryLoader(new LibraryLoader(this)),
      m_currentGameType(other.m_currentGameType),
      m_gameTimer(new QTimer(this)),
      m_gameStarted(other.m_gameStarted),
      m_gamePaused(other.m_gamePaused),
      m_pressedKeys(other.m_pressedKeys) {
  connect(m_gameTimer, &QTimer::timeout, this, &GameController::updateGame);
  
  if (other.m_libraryLoader->isLoaded()) {
    m_libraryLoader->loadGame(other.m_currentGameType);
  }
}

GameController& GameController::operator=(const GameController& other) {
  if (this != &other) {
    QObject::operator=(other);
    
    m_currentGameType = other.m_currentGameType;
    m_gameStarted = other.m_gameStarted;
    m_gamePaused = other.m_gamePaused;
    m_pressedKeys = other.m_pressedKeys;
    
    if (other.m_libraryLoader->isLoaded()) {
      m_libraryLoader->loadGame(other.m_currentGameType);
    } else {
      m_libraryLoader->unloadGame();
    }
    
    if (m_gameStarted && !m_gamePaused) {
      m_gameTimer->start(600);
    } else {
      m_gameTimer->stop();
    }
  }
  return *this;
}

GameController::GameController(GameController&& other) noexcept
    : QObject(other.parent()),
      m_libraryLoader(other.m_libraryLoader),
      m_currentGameType(other.m_currentGameType),
      m_gameTimer(other.m_gameTimer),
      m_gameStarted(other.m_gameStarted),
      m_gamePaused(other.m_gamePaused),
      m_pressedKeys(std::move(other.m_pressedKeys)) {
  
  if (m_libraryLoader) {
    m_libraryLoader->setParent(this);
  }
  if (m_gameTimer) {
    m_gameTimer->setParent(this);
    connect(m_gameTimer, &QTimer::timeout, this, &GameController::updateGame);
  }
  
  other.m_libraryLoader = nullptr;
  other.m_gameTimer = nullptr;
  other.m_gameStarted = false;
  other.m_gamePaused = false;
}

GameController& GameController::operator=(GameController&& other) noexcept {
  if (this != &other) {
    unloadGame();
    
    m_libraryLoader = other.m_libraryLoader;
    m_gameTimer = other.m_gameTimer;
    m_currentGameType = other.m_currentGameType;
    m_gameStarted = other.m_gameStarted;
    m_gamePaused = other.m_gamePaused;
    m_pressedKeys = std::move(other.m_pressedKeys);
    
    if (m_libraryLoader) {
      m_libraryLoader->setParent(this);
    }
    if (m_gameTimer) {
      m_gameTimer->setParent(this);
      connect(m_gameTimer, &QTimer::timeout, this, &GameController::updateGame);
    }
    
    other.m_libraryLoader = nullptr;
    other.m_gameTimer = nullptr;
    other.m_gameStarted = false;
    other.m_gamePaused = false;
  }
  return *this;
}
