#pragma once
#include <QTimer>
#include <QWidget>

#include "../../brickgame/common/types.h"

typedef void (*UserInputFunc)(UserAction_t, bool);
typedef GameInfo_t (*UpdateStateFunc)();
typedef bool (*IsGameOverFunc)();

class GameWidget : public QWidget {
  Q_OBJECT
 public:
  GameWidget(QWidget* parent = nullptr);

  void setGameAPI(UserInputFunc userInput, UpdateStateFunc updateState,
                  IsGameOverFunc isGameOver);

 protected:
  void paintEvent(QPaintEvent*) override;
  void keyPressEvent(QKeyEvent*) override;

 private slots:
  void gameTick();

 private:
  QTimer* timer;
  GameInfo_t gameInfo;
  bool started = false;
  bool paused = false;

  UserInputFunc userInputFunc = nullptr;
  UpdateStateFunc updateStateFunc = nullptr;
  IsGameOverFunc isGameOverFunc = nullptr;

  void sendUserInput(UserAction_t action, bool hold = false);
  void updateGame();
  void renderGameOverScreen(QPainter& p);
  void renderStartScreen(QPainter& p);
};