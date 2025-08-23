#ifndef GAMESELECTIONDIALOG_H
#define GAMESELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "gamecontroller.h"

class GameSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameSelectionDialog(QWidget *parent = nullptr);
    ~GameSelectionDialog();

signals:
    void gameSelected(GameType gameType);

private slots:
    void onTetrisSelected();
    void onSnakeSelected();

private:
    void setupUI();
    
    QPushButton* m_tetrisButton;
    QPushButton* m_snakeButton;
    QLabel* m_titleLabel;
};

#endif // GAMESELECTIONDIALOG_H
