#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include "gamecontroller.h"
#include "gamewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onGameStateChanged(const GameInfo_t& state);
    void onGameOver();
    void onGameWon();
    void onGamePaused();
    void onGameResumed();
    void showGameSelection();
    void startSelectedGame(GameType gameType);
    void restartCurrentGame();
    
    // Слоты для кнопок
    void onStartButtonClicked();
    void onPauseButtonClicked();
    void onQuitButtonClicked();

private:
    void setupUI();
    void setupGameWidget();
    void setupInfoPanel();
    void updateInfoPanel(const GameInfo_t& state);
    void showStartScreen();
    void showGameOverScreen();
    void showGameWonScreen();
    void setupConnections();
    void drawNextFigure(const GameInfo_t& state);

    GameController* m_gameController;
    GameWidget* m_gameWidget;
    
    // Информационная панель
    QWidget* m_infoPanel;
    QLabel* m_scoreLabel;
    QLabel* m_highScoreLabel;
    QLabel* m_levelLabel;
    QLabel* m_nextLabel;
    QWidget* m_nextFigureWidget;

    
    // Кнопки навигации
    QPushButton* m_startButton;
    QPushButton* m_pauseButton;
    QPushButton* m_quitButton;
    
    // Центральный виджет
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    
    bool m_gameStarted;
    bool m_gamePaused;
    GameType m_currentGameType;
};

#endif // MAINWINDOW_H
