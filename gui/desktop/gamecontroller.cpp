/**
 * @file gamecontroller.cpp
 * @brief Реализация тонкого контроллера игры.
 */

#include "../../include/gui/desktop/gamecontroller.h"

#include <QDebug>

GameController::GameController(QObject* parent)
    : QObject(parent),
      m_libraryLoader(std::make_unique<LibraryLoader>(this)),
      m_inputHandler(std::make_unique<InputHandler>(this)),
      m_timerManager(std::make_unique<TimerManager>(this)),
      m_currentGameType(GameType::TETRIS),
      m_wasPaused(false) {
  setupConnections();
}

GameController::~GameController() { unloadGame(); }

bool GameController::loadGame(GameType gameType) {
  unloadGame();

  m_currentGameType = gameType;
  m_inputHandler->setGameType(gameType);
  m_wasPaused = false;

  return m_libraryLoader->loadGame(gameType);
}

void GameController::unloadGame() {
  m_timerManager->stop();
  m_inputHandler->clear();
  m_libraryLoader->unloadGame();
}

void GameController::handleKeyPress(int key) {
  m_inputHandler->handleKeyPress(key, m_libraryLoader.get());
}

void GameController::handleKeyRelease(int key) {
  m_inputHandler->handleKeyRelease(key);
}

void GameController::handleKeyReleased(int key) {
  if (m_currentGameType == GameType::SNAKE) {
    UserAction_t action = m_inputHandler->mapKeyToAction(key);
    if (action == Up || action == Down || action == Left || action == Right) {
      if (m_libraryLoader->isLoaded()) {
        GameAPI api = m_libraryLoader->getAPI();
        api.userInput(Action, false);
      }
    }
  }
}

void GameController::handleAction(UserAction_t action) {
  if (!m_libraryLoader->isLoaded()) {
    return;
  }

  GameAPI api = m_libraryLoader->getAPI();

  switch (action) {
    case Start:
      api.userInput(Start, false);
      m_timerManager->start();
      updateGameState();
      break;

    case Pause: {
      api.userInput(Pause, false);

      GameInfo_t currentState = api.updateCurrentState();

      if (currentState.pause && !m_wasPaused) {
        m_timerManager->stop();
        emit gamePaused();
        m_wasPaused = true;
      } else if (!currentState.pause && m_wasPaused) {
        m_timerManager->start();
        emit gameResumed();
        m_wasPaused = false;
      }

      emit gameStateChanged(currentState);
      break;
    }

    case Terminate:
      api.userInput(Terminate, false);
      m_timerManager->stop();
      emit applicationCloseRequested();
      break;

    default:
      break;
  }
}

GameInfo_t GameController::getCurrentState() const {
  if (!m_libraryLoader->isLoaded()) {
    GameInfo_t empty = {0};
    return empty;
  }
  GameAPI api = m_libraryLoader->getAPI();
  return api.updateCurrentState();
}

bool GameController::isGameOver() const {
  if (!m_libraryLoader->isLoaded()) return false;
  GameAPI api = m_libraryLoader->getAPI();
  return api.isOver();
}

bool GameController::isGameStarted() const {
  if (!m_libraryLoader->isLoaded()) return false;
  GameAPI api = m_libraryLoader->getAPI();
  GameInfo_t state = api.updateCurrentState();
  bool result = !state.pause && !api.isOver();
  return result;
}

bool GameController::isGamePaused() const {
  if (!m_libraryLoader->isLoaded()) return false;
  GameAPI api = m_libraryLoader->getAPI();
  GameInfo_t state = api.updateCurrentState();
  bool result = state.pause;
  return result;
}

GameType GameController::getCurrentGameType() const {
  return m_currentGameType;
}

void GameController::updateGame() {
  if (!m_libraryLoader->isLoaded()) {
    return;
  }

  try {
    GameAPI api = m_libraryLoader->getAPI();
    GameInfo_t currentState = api.updateCurrentState();

    if (!currentState.field) {
      return;
    }

    if (currentState.speed != m_timerManager->getInterval()) {
      m_timerManager->setInterval(currentState.speed);
    }

    emit gameStateChanged(currentState);

    if (api.isOver()) {
      m_timerManager->stop();
      emit gameOver();
    }
  } catch (...) {
    m_timerManager->stop();
  }
}

void GameController::setupConnections() {
  connect(m_inputHandler.get(), &InputHandler::actionRequested, this,
          &GameController::handleAction);
  connect(m_inputHandler.get(), &InputHandler::keyReleased, this,
          &GameController::handleKeyReleased);

  connect(m_timerManager.get(), &TimerManager::timeout, this,
          &GameController::updateGame);
}

void GameController::updateGameState() {
  if (m_libraryLoader->isLoaded()) {
    GameAPI api = m_libraryLoader->getAPI();
    GameInfo_t initialState = api.updateCurrentState();
    if (initialState.field) {
      emit gameStateChanged(initialState);
    }
  }
}

void GameController::showGameSelection() { emit showGameSelectionRequested(); }

void GameController::stopGame() {
  if (m_libraryLoader->isLoaded()) {
    GameAPI api = m_libraryLoader->getAPI();
    api.userInput(Terminate, false);
  }
  m_timerManager->stop();
}

void GameController::closeApplication() { emit applicationCloseRequested(); }

void GameController::handleRestartGame() {
  if (m_libraryLoader->isLoaded()) {
    GameAPI api = m_libraryLoader->getAPI();
    GameInfo_t state = api.updateCurrentState();
    api.userInput(Start, false);
    m_timerManager->start();
    updateGameState();
  }
}

void GameController::handleGameSelection(GameType gameType) {
  if (loadGame(gameType)) {
    emit gameSelected(gameType);
  }
}

void GameController::handleStartButton() { handleAction(Start); }

void GameController::handlePauseButton() { handleAction(Pause); }

void GameController::handleQuitButton() { handleAction(Terminate); }
