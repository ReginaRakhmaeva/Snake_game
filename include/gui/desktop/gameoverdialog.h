#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

class GameOverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameOverDialog(bool isWon = false, QWidget *parent = nullptr);
    ~GameOverDialog();

signals:
    void restartRequested();
    void quitRequested();

private slots:
    void onRestartClicked();
    void onQuitClicked();

private:
    void setupUI(bool isWon);
    
    QPushButton* m_restartButton;
    QPushButton* m_quitButton;
    QLabel* m_messageLabel;
};

#endif // GAMEOVERDIALOG_H
