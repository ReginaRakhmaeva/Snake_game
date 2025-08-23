#include "../../../include/gui/desktop/gamewidget.h"
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QtMath>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , m_currentScreen(ScreenType::START)
{
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet("QWidget { background-color: black; }");
}

GameWidget::~GameWidget()
{
}

void GameWidget::updateGameState(const GameInfo_t& state)
{
    m_currentState = state;
    m_currentScreen = ScreenType::GAME;
    update();
}

void GameWidget::showStartScreen()
{
    m_currentScreen = ScreenType::START;
    update();
}

void GameWidget::showGameOverScreen()
{
    m_currentScreen = ScreenType::GAME_OVER;
    update();
}

void GameWidget::showGameWonScreen()
{
    m_currentScreen = ScreenType::GAME_WON;
    update();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Очищаем фон
    painter.fillRect(rect(), QColor(44, 62, 80)); // Темно-серый фон
    
    switch (m_currentScreen) {
        case ScreenType::START:
            drawStartScreen(painter);
            break;
        case ScreenType::GAME:
            drawGameField(painter);
            break;
        case ScreenType::GAME_OVER:
            drawGameOverScreen(painter);
            break;
        case ScreenType::GAME_WON:
            drawGameWonScreen(painter);
            break;
    }
}

void GameWidget::drawGameField(QPainter& painter)
{
    if (!m_currentState.field) return;
    
    // Рисуем границы
    drawBorders(painter);
    
    // Рисуем поле
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            if (m_currentState.field[y] && m_currentState.field[y][x]) {
                QRect cellRect = getCellRect(x, y);
                painter.fillRect(cellRect, getCellColor(m_currentState.field[y][x]));
                painter.setPen(QPen(Qt::white, 1));
                painter.drawRect(cellRect);
            }
        }
    }
}

void GameWidget::drawStartScreen(QPainter& painter)
{
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    
    // Заголовок
    QString title = "BRICKGAME";
    QFontMetrics fm(painter.font());
    int titleWidth = fm.horizontalAdvance(title);
    painter.drawText((width() - titleWidth) / 2, height() / 2 - 120, title);
    
    // Инструкции
    painter.setFont(QFont("Arial", 16, QFont::Normal));
    QStringList instructions = {
        "Press ENTER to Start",
        "or Q to Quit",
        "",
        "CONTROLS",
        "Arrow Keys: Move",
        "Space/Action: Rotate/Speed up",
        "P: Pause",
        "Q: Quit"
    };
    
    int y = height() / 2 - 40;
    for (const QString& instruction : instructions) {
        if (!instruction.isEmpty()) {
            int textWidth = fm.horizontalAdvance(instruction);
            painter.drawText((width() - textWidth) / 2, y, instruction);
        }
        y += 30;
    }
}

void GameWidget::drawGameOverScreen(QPainter& painter)
{
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    
    QString title = "GAME OVER";
    QFontMetrics fm(painter.font());
    int titleWidth = fm.horizontalAdvance(title);
    painter.drawText((width() - titleWidth) / 2, height() / 2 - 60, title);
    
    painter.setFont(QFont("Arial", 16, QFont::Normal));
    QStringList messages = {
        "Press ENTER to restart",
        "Press Q to exit"
    };
    
    int y = height() / 2;
    for (const QString& message : messages) {
        int textWidth = fm.horizontalAdvance(message);
        painter.drawText((width() - textWidth) / 2, y, message);
        y += 30;
    }
}

void GameWidget::drawGameWonScreen(QPainter& painter)
{
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    
    QString title = "YOU WON!";
    QFontMetrics fm(painter.font());
    int titleWidth = fm.horizontalAdvance(title);
    painter.drawText((width() - titleWidth) / 2, height() / 2 - 60, title);
    
    painter.setFont(QFont("Arial", 16, QFont::Normal));
    QStringList messages = {
        "Press ENTER to restart",
        "Press Q to exit"
    };
    
    int y = height() / 2;
    for (const QString& message : messages) {
        int textWidth = fm.horizontalAdvance(message);
        painter.drawText((width() - textWidth) / 2, y, message);
        y += 30;
    }
}

void GameWidget::drawBorders(QPainter& painter)
{
    // Вычисляем размер ячейки, чтобы они были квадратными
    int cellSize = qMin(width() / FIELD_WIDTH, height() / FIELD_HEIGHT);
    
    // Центрируем поле в виджете
    int offsetX = (width() - FIELD_WIDTH * cellSize) / 2;
    int offsetY = (height() - FIELD_HEIGHT * cellSize) / 2;
    
    painter.setPen(QPen(Qt::white, BORDER_WIDTH));
    
    // Внешние границы игрового поля
    QRect borderRect(offsetX, offsetY, FIELD_WIDTH * cellSize, FIELD_HEIGHT * cellSize);
    painter.drawRect(borderRect);
    
    // Внутренние линии (опционально)
    painter.setPen(QPen(Qt::gray, 1));
    
    for (int x = 1; x < FIELD_WIDTH; ++x) {
        painter.drawLine(offsetX + x * cellSize, offsetY, 
                        offsetX + x * cellSize, offsetY + FIELD_HEIGHT * cellSize);
    }
    for (int y = 1; y < FIELD_HEIGHT; ++y) {
        painter.drawLine(offsetX, offsetY + y * cellSize, 
                        offsetX + FIELD_WIDTH * cellSize, offsetY + y * cellSize);
    }
}

QColor GameWidget::getCellColor(int cellValue) const
{
    switch (cellValue) {
        case 1: // Snake body
            return QColor(0, 255, 0); // Green
        case 2: // Apple
            return QColor(255, 0, 0); // Red
        default:
            return QColor(128, 128, 128); // Gray
    }
}

QRect GameWidget::getCellRect(int x, int y) const
{
    // Вычисляем размер ячейки, чтобы они были квадратными
    int cellSize = qMin(width() / FIELD_WIDTH, height() / FIELD_HEIGHT);
    
    // Центрируем поле в виджете
    int offsetX = (width() - FIELD_WIDTH * cellSize) / 2;
    int offsetY = (height() - FIELD_HEIGHT * cellSize) / 2;
    
    return QRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
}
