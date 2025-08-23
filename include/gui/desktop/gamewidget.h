#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "../../brickgame/common/types.h"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void updateGameState(const GameInfo_t& state);
    void showStartScreen();
    void showGameOverScreen();
    void showGameWonScreen();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawGameField(QPainter& painter);
    void drawStartScreen(QPainter& painter);
    void drawGameOverScreen(QPainter& painter);
    void drawGameWonScreen(QPainter& painter);
    void drawBorders(QPainter& painter);
    
    QColor getCellColor(int cellValue) const;
    QRect getCellRect(int x, int y) const;

    GameInfo_t m_currentState;
    enum class ScreenType {
        START,
        GAME,
        GAME_OVER,
        GAME_WON
    } m_currentScreen;
    
    static const int CELL_SIZE = 25;
    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 20;
    static const int BORDER_WIDTH = 2;
};

#endif // GAMEWIDGET_H
