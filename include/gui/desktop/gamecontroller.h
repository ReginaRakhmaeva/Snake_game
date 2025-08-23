#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <dlfcn.h>
#include <memory>
#include "../../brickgame/common/types.h"

// Перечисление поддерживаемых игр
enum class GameType {
    TETRIS,
    SNAKE
};

// Структура для работы с API игры
struct GameAPI {
    void* lib_handle = nullptr;
    void (*userInput)(UserAction_t action, bool hold) = nullptr;
    GameInfo_t (*updateState)(void) = nullptr;
    bool (*isOver)(void) = nullptr;
    bool valid = false;
    QString error;
};

class GameController : public QObject
{
    Q_OBJECT

public:
    explicit GameController(QObject *parent = nullptr);
    ~GameController();

    bool loadGame(GameType gameType);
    void unloadGame();
    void startGame();
    void pauseGame();
    void resumeGame();
    void stopGame();
    void processInput(UserAction_t action, bool hold = false);
    GameInfo_t getCurrentState() const;
    bool isGameOver() const;
    bool isGameStarted() const;
    bool isGamePaused() const;
    GameType getCurrentGameType() const;
    UserAction_t mapKeyToAction(int key) const;

signals:
    void gameStateChanged(const GameInfo_t& state);
    void gameOver();
    void gameWon();
    void gamePaused();
    void gameResumed();

private slots:
    void updateGame();

private:
    GameAPI m_api;
    GameType m_currentGameType;
    QTimer* m_gameTimer;
    bool m_gameStarted;
    bool m_gamePaused;
    
    QString getLibraryPath(GameType gameType) const;
};

#endif // GAMECONTROLLER_H
