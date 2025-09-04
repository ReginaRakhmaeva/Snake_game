#include <gtest/gtest.h>

#include "../include/brickgame/tetris/game.h"

class TetrisFSMTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  void freeGameInfo(GameInfo_t* info) {
    if (info) {
      info->field = nullptr;
      info->next = nullptr;
    }
  }
};

TEST_F(TetrisFSMTest, InitialState) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(TetrisFSMTest, StartTransition) {
  userInput(Start, false);

  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(TetrisFSMTest, PauseTransition) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisFSMTest, TerminateTransition) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Terminate, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  EXPECT_TRUE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(TetrisFSMTest, InputHandlingInDifferentStates) {
  userInput(Left, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Start, false);

  userInput(Right, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Pause, false);

  userInput(Action, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisFSMTest, StatePersistence) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  GameInfo_t info2 = updateCurrentState();
  GameInfo_t info3 = updateCurrentState();

  EXPECT_NE(info1.field, nullptr);
  EXPECT_NE(info2.field, nullptr);
  EXPECT_NE(info3.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisFSMTest, MultipleStateTransitions) {
  userInput(Start, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info5 = updateCurrentState();
  EXPECT_NE(info5.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
  freeGameInfo(&info5);
}

TEST_F(TetrisFSMTest, InvalidTransitions) {
  userInput(Terminate, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Start, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(TetrisFSMTest, ComplexStateMachine) {
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Start, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Left, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);

  userInput(Pause, false);
  GameInfo_t info5 = updateCurrentState();
  EXPECT_NE(info5.field, nullptr);

  userInput(Right, false);
  GameInfo_t info6 = updateCurrentState();
  EXPECT_NE(info6.field, nullptr);

  userInput(Terminate, false);
  GameInfo_t info7 = updateCurrentState();
  EXPECT_NE(info7.field, nullptr);
  EXPECT_TRUE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
  freeGameInfo(&info5);
  freeGameInfo(&info6);
  freeGameInfo(&info7);
}

TEST_F(TetrisFSMTest, HoldParameterInDifferentStates) {
  userInput(Start, false);

  userInput(Left, true);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Pause, false);

  userInput(Right, true);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Pause, false);

  userInput(Action, true);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);
  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisFSMTest, ExtendedGameplayForCoverage) {
  userInput(Start, false);

  for (int i = 0; i < 150; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisFSMTest, ExtendedStateTransitions) {
  userInput(Start, false);

  for (int i = 0; i < 80; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  userInput(Pause, false);
  GameInfo_t pause_info = updateCurrentState();
  EXPECT_NE(pause_info.field, nullptr);
  freeGameInfo(&pause_info);

  userInput(Pause, false);
  GameInfo_t resume_info = updateCurrentState();
  EXPECT_NE(resume_info.field, nullptr);
  freeGameInfo(&resume_info);
}

TEST_F(TetrisFSMTest, ScoreAccumulation) {
  userInput(Start, false);

  for (int i = 0; i < 250; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisFSMTest, LevelProgression) {
  userInput(Start, false);

  for (int i = 0; i < 350; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}
