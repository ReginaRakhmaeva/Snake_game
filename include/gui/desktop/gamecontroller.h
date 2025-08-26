#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <memory>
#include "../../brickgame/common/types.h"
#include "libraryloader.h"

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
    void handleKeyPress(int key);
    void handleStartButton();
    void handlePauseButton();
    void handleQuitButton();
    void handleGameSelection(GameType gameType);
    void handleRestartGame();
    void showGameSelection();
    void closeApplication();

signals: 
    void gameStateChanged(const GameInfo_t& state);
    void gameOver();
    void gameWon();
    void gamePaused();
    void gameResumed();
    void gameSelected(GameType gameType);
    void showGameSelectionRequested();
    void applicationCloseRequested();

private slots:
    void updateGame();

private:
    LibraryLoader* m_libraryLoader;
    GameType m_currentGameType;
    QTimer* m_gameTimer;
    bool m_gameStarted;
    bool m_gamePaused;
};

#endif // GAMECONTROLLER_H
