/**
 * @file inputhandler.cpp
 * @brief Implementation of the input handler.
 */

#include "../../include/gui/desktop/inputhandler.h"

#include <QDebug>

InputHandler::InputHandler(QObject* parent)
    : QObject(parent), m_currentGameType(GameType::TETRIS) {}

void InputHandler::setGameType(GameType gameType) {
  m_currentGameType = gameType;
  clear();
}

void InputHandler::handleKeyPress(int key, LibraryLoader* libraryLoader) {
  m_pressedKeys.insert(key);

  UserAction_t action = mapKeyToAction(key);
  if (action == static_cast<UserAction_t>(-1)) {
    return;
  }

  switch (action) {
    case Start:
    case Pause:
    case Terminate:
      emit actionRequested(action);
      return;
    default:
      break;
  }

  if (m_currentGameType == GameType::TETRIS && !checkDebounce(key)) {
    return;
  }

  if (libraryLoader && libraryLoader->isLoaded()) {
    bool hold = false;
    if (m_currentGameType == GameType::SNAKE) {
      hold = m_pressedKeys.contains(key);

      if (action == Up || action == Down || action == Left || action == Right) {
        GameAPI api = libraryLoader->getAPI();
        api.userInput(action, hold);

        api.userInput(Action, hold);
      } else {
        GameAPI api = libraryLoader->getAPI();
        api.userInput(action, hold);
      }
    } else {
      GameAPI api = libraryLoader->getAPI();
      api.userInput(action, hold);
    }
  }
}

void InputHandler::handleKeyRelease(int key) {
  m_pressedKeys.remove(key);

  if (m_currentGameType == GameType::SNAKE) {
    UserAction_t action = mapKeyToAction(key);
    if (action == Up || action == Down || action == Left || action == Right) {
      emit keyReleased(key);
    }
  }
}

void InputHandler::clear() {
  m_pressedKeys.clear();
  m_lastKeyPressTime.clear();
}

UserAction_t InputHandler::mapKeyToAction(int key) const {
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

bool InputHandler::checkDebounce(int key) {
  qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
  qint64 lastTime = m_lastKeyPressTime.value(key, 0);

  if (currentTime - lastTime < KEY_DEBOUNCE_MS) {
    return false;
  }

  m_lastKeyPressTime[key] = currentTime;
  return true;
}
