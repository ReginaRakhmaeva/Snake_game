#include "../../../include/gui/desktop/gamecontroller.h"
#include <QApplication>
#include <QDebug>
#include <QDir>

GameController::GameController(QObject *parent)
    : QObject(parent)
    , m_currentGameType(GameType::TETRIS)
    , m_gameTimer(new QTimer(this))
    , m_gameStarted(false)
    , m_gamePaused(false)
{
    // Настройка таймера игры
    connect(m_gameTimer, &QTimer::timeout, this, &GameController::updateGame);
}

GameController::~GameController()
{
    unloadGame();
}

bool GameController::loadGame(GameType gameType)
{
    unloadGame();
    
    m_currentGameType = gameType;
    QString libPath = getLibraryPath(gameType);
    
    // Загружаем библиотеку
    m_api.lib_handle = dlopen(libPath.toUtf8().constData(), RTLD_LAZY);
    if (!m_api.lib_handle) {
        m_api.error = QString("Failed to load library: %1").arg(dlerror());
        qDebug() << "Error loading library:" << m_api.error;
        return false;
    }
    
    // Загружаем функции API
    m_api.userInput = (void (*)(UserAction_t, bool))dlsym(m_api.lib_handle, "userInput");
    m_api.updateState = (GameInfo_t (*)(void))dlsym(m_api.lib_handle, "updateCurrentState");
    m_api.isOver = (bool (*)(void))dlsym(m_api.lib_handle, "isGameOver");
    
    // Проверяем успешность загрузки функций
    if (!m_api.userInput || !m_api.updateState || !m_api.isOver) {
        m_api.error = "Failed to load required functions";
        qDebug() << "Error loading functions:" << m_api.error;
        dlclose(m_api.lib_handle);
        m_api.lib_handle = nullptr;
        return false;
    }
    
    m_api.valid = true;
    qDebug() << "Game library loaded successfully:" << libPath;
    return true;
}

void GameController::unloadGame()
{
    if (m_api.lib_handle) {
        dlclose(m_api.lib_handle);
        m_api.lib_handle = nullptr;
    }
    m_api.valid = false;
    m_api.error.clear();
    m_gameStarted = false;
    m_gamePaused = false;
    m_gameTimer->stop();
}

void GameController::startGame()
{
    if (!m_api.valid) return;
    
    try {
        m_gameStarted = true;
        m_gamePaused = false;
        
        // Инициализируем игру
        m_api.userInput(Start, false);
        
        // Запускаем таймер с базовой скоростью
        m_gameTimer->start(600); // Базовая скорость
        
        // Получаем начальное состояние
        GameInfo_t initialState = m_api.updateState();
        if (initialState.field) {
            emit gameStateChanged(initialState);
        }
    } catch (...) {
        qDebug() << "Exception in startGame()";
        m_gameStarted = false;
    }
}

void GameController::pauseGame()
{
    if (!m_gameStarted || m_gamePaused) return;
    
    m_gamePaused = true;
    m_gameTimer->stop();
    emit gamePaused();
}

void GameController::resumeGame()
{
    if (!m_gameStarted || !m_gamePaused) return;
    
    m_gamePaused = false;
    // Используем базовую скорость, обновление произойдет в updateGame()
    m_gameTimer->start(600);
    emit gameResumed();
}

void GameController::stopGame()
{
    m_gameStarted = false;
    m_gamePaused = false;
    m_gameTimer->stop();
}

void GameController::processInput(UserAction_t action, bool hold)
{
    if (!m_api.valid) return;
    
    m_api.userInput(action, hold);
}

GameInfo_t GameController::getCurrentState() const
{
    if (!m_api.valid) {
        GameInfo_t empty = {0};
        return empty;
    }
    return m_api.updateState();
}

bool GameController::isGameOver() const
{
    if (!m_api.valid) return false;
    return m_api.isOver();
}

bool GameController::isGameStarted() const
{
    return m_gameStarted;
}

bool GameController::isGamePaused() const
{
    return m_gamePaused;
}

GameType GameController::getCurrentGameType() const
{
    return m_currentGameType;
}



void GameController::updateGame()
{
    if (!m_api.valid || !m_gameStarted || m_gamePaused) return;
    
    try {
        // Получаем текущее состояние игры
        GameInfo_t currentState = m_api.updateState();
        
        // Проверяем валидность указателей
        if (!currentState.field) {
            qDebug() << "Warning: currentState.field is null";
            return;
        }
        
        // Обновляем скорость таймера если она изменилась
        if (currentState.speed != m_gameTimer->interval()) {
            m_gameTimer->setInterval(currentState.speed);
        }
        
        emit gameStateChanged(currentState);
        
        // Проверяем окончание игры
        if (m_api.isOver()) {
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

QString GameController::getLibraryPath(GameType gameType) const
{
    QString libName;
    QString baseName;
    
    switch (gameType) {
        case GameType::TETRIS:
            baseName = "libtetris";
            break;
        case GameType::SNAKE:
            baseName = "libsnake";
            break;
    }
    
    // Определяем расширение библиотеки в зависимости от ОС
#ifdef Q_OS_MACOS
    libName = baseName + ".dylib";
#elif defined(Q_OS_WIN)
    libName = baseName + ".dll";
#else
    libName = baseName + ".so";
#endif
    
    // Ищем библиотеку в текущей директории
    QDir currentDir = QDir::current();
    QString libPath = currentDir.absoluteFilePath(libName);
    
    if (!QFile::exists(libPath)) {
        // Если не найдена, ищем в родительской директории
        libPath = currentDir.absoluteFilePath("../" + libName);
    }
    
    return libPath;
}

UserAction_t GameController::mapKeyToAction(int key) const
{
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
            return static_cast<UserAction_t>(-1); // Неизвестная клавиша
    }
}

// Новые методы для управления игрой
void GameController::selectGame(GameType gameType)
{
    if (loadGame(gameType)) {
        emit gameSelected(gameType);
    }
}

void GameController::restartGame()
{
    if (loadGame(m_currentGameType)) {
        emit gameSelected(m_currentGameType);
    }
}

void GameController::handleStartButton()
{
    if (!m_gameStarted) {
        startGame();
    }
}

void GameController::handlePauseButton()
{
    if (m_gameStarted && !m_gamePaused) {
        pauseGame();
    } else if (m_gameStarted && m_gamePaused) {
        resumeGame();
    }
}

void GameController::handleQuitButton()
{
    if (m_gameStarted) {
        stopGame();
    }
    closeApplication();
}

void GameController::showGameSelection()
{
    emit showGameSelectionRequested();
}

void GameController::closeApplication()
{
    emit applicationCloseRequested();
}
