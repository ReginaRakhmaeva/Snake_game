#pragma once
#include <QMainWindow>

class GameWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void setSnake();
    void setTetris();
private:
    GameWidget* gameWidget;
};