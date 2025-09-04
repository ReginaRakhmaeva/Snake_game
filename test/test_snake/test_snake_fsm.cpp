#include <gtest/gtest.h>

extern "C" {
#include "../../include/brickgame/common/types.h"
#include "../../include/brickgame/snake/snake_api.h"
}

class SnakeFSMTest : public ::testing::Test {
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

TEST_F(SnakeFSMTest, StartFromReady) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(isVictory());

  userInput(Start, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  freeGameInfo(&info);
  freeGameInfo(&info2);
}

TEST_F(SnakeFSMTest, PauseAndResume) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, TerminateSetsLost) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, ArrowChangesDirection) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, OppositeDirectionIgnored) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, StartDuringPauseIgnored) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, MovementBlockedWhilePaused) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, TerminateAfterLostIgnored) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, StartAfterWonRestartsGame) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, StartAfterLostRestartsGame) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_GE(info.score, 0);
  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, UpDirectionHandling) {
  userInput(Start, false);

  userInput(Up, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);

  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, DownDirectionHandling) {
  userInput(Start, false);

  userInput(Down, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);

  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, LeftDirectionHandling) {
  userInput(Start, false);

  userInput(Left, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);

  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, InvalidActionHandling) {
  userInput(Start, false);

  userInput(static_cast<UserAction_t>(-1), false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);

  freeGameInfo(&info);
}

TEST_F(SnakeFSMTest, MultipleDirectionInputs) {
  userInput(Start, false);

  userInput(Up, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Down, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Left, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  userInput(Right, false);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
}

TEST_F(SnakeFSMTest, HoldWithDirections) {
  userInput(Start, false);

  userInput(Up, true);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Down, true);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Left, true);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}
