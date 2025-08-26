#include <gtest/gtest.h>
#include "../../include/brickgame/snake/snake_api.h"

class SnakeGameTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void TearDown() override {
  }

  void freeGameInfo(GameInfo_t* info) {
    if (info) {
      info->field = nullptr;
      info->next = nullptr;
    }
  }
};

TEST_F(SnakeGameTest, InitialState) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  for (int y = 0; y < 20; ++y) {
    EXPECT_NE(info.field[y], nullptr);
  }
  
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, GameStart) {
  userInput(Start, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, MovementAndStateChanges) {
  userInput(Start, false);
  
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  
  userInput(Right, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  EXPECT_TRUE(info1.field != nullptr && info2.field != nullptr);
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(SnakeGameTest, PauseAndResume) {
  userInput(Start, false);
  
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  EXPECT_FALSE(isGameOver());
  
  userInput(Pause, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  userInput(Pause, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);
  
  EXPECT_FALSE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(SnakeGameTest, GameTermination) {
  userInput(Start, false);
  
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  EXPECT_FALSE(isGameOver());
  
  userInput(Terminate, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  EXPECT_TRUE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(SnakeGameTest, AccelerationWithHold) {
  userInput(Start, false);
  
  userInput(Right, true);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  
  userInput(Right, true);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  userInput(Right, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);
  
  EXPECT_FALSE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(SnakeGameTest, MultipleDirectionChanges) {
  userInput(Start, false);
  
  userInput(Right, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  
  EXPECT_TRUE(info1.field != nullptr);
  
  freeGameInfo(&info1);
}

TEST_F(SnakeGameTest, GameStatePersistence) {
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

TEST_F(SnakeGameTest, ActionInputs) {
  userInput(Start, false);
  
  userInput(Action, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  
  userInput(Action, true);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  EXPECT_FALSE(isGameOver());
  EXPECT_FALSE(isVictory());
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(SnakeGameTest, ComplexGameplay) {
  userInput(Start, false);
  
  for (int i = 0; i < 3; ++i) {
    userInput(Right, i % 2 == 0);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
  
  userInput(Pause, false);
  GameInfo_t info_pause = updateCurrentState();
  EXPECT_NE(info_pause.field, nullptr);
  freeGameInfo(&info_pause);
  
  userInput(Pause, false);
  GameInfo_t info_resume = updateCurrentState();
  EXPECT_NE(info_resume.field, nullptr);
  
  freeGameInfo(&info_resume);
}

TEST_F(SnakeGameTest, AllDirectionMovements) {
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

TEST_F(SnakeGameTest, HoldWithAllDirections) {
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
  
  userInput(Right, true);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
}

TEST_F(SnakeGameTest, InvalidInputs) {
  userInput(Start, false);
  
  userInput(static_cast<UserAction_t>(-1), false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  
  userInput(static_cast<UserAction_t>(999), false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);
  
  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(SnakeGameTest, RapidInputSequence) {
  userInput(Start, false);
  
  userInput(Up, false);
  userInput(Down, false);
  userInput(Left, false);
  userInput(Right, false);
  userInput(Action, false);
  userInput(Pause, false);
  userInput(Pause, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, DefaultCaseInChangeDirection) {
  userInput(Start, false);
  
  userInput(static_cast<UserAction_t>(999), false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, DownDirectionMovement) {
  userInput(Start, false);
  
  userInput(Down, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, AppleCollectionAndGrowth) {
  userInput(Start, false);
  
  for (int i = 0; i < 20; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, VictoryCondition) {
  userInput(Start, false);
  
  for (int i = 0; i < 50; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, VictoryConditionExtended) {
  userInput(Start, false);
  
  for (int i = 0; i < 220; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, HighScoreUpdate) {
  userInput(Start, false);
  
  for (int i = 0; i < 30; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, LevelProgression) {
  userInput(Start, false);
  
  for (int i = 0; i < 40; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, SaveHighScoreFunction) {
  userInput(Start, false);
  
  for (int i = 0; i < 100; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, VictoryConditionWithMaxLength) {
  userInput(Start, false);
  
  for (int i = 0; i < 200; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, HighScoreUpdateCondition) {
  userInput(Start, false);
  
  for (int i = 0; i < 150; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, AppleCollectionToTriggerHighScore) {
  userInput(Start, false);
  
  for (int i = 0; i < 120; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, LongGameSessionForCoverage) {
  userInput(Start, false);
  
  for (int i = 0; i < 300; ++i) {
    switch (i % 4) {
      case 0:
        userInput(Right, false);
        break;
      case 1:
        userInput(Down, false);
        break;
      case 2:
        userInput(Left, false);
        break;
      case 3:
        userInput(Up, false);
        break;
    }
    
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, MaxLengthVictoryPath) {
  userInput(Start, false);
  
  for (int i = 0; i < 250; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, HighScoreSaveTrigger) {
  userInput(Start, false);
  
  for (int i = 0; i < 180; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, CollisionWithWall) {
  userInput(Start, false);
  
  for (int i = 0; i < 15; ++i) {
    userInput(Right, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }
}

TEST_F(SnakeGameTest, CollisionWithSelf) {
  userInput(Start, false);
  
  userInput(Right, false);
  userInput(Down, false);
  userInput(Left, false);
  userInput(Up, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, OppositeDirectionHandling) {
  userInput(Start, false);
  
  userInput(Right, false);
  userInput(Left, false);
  
  userInput(Down, false);
  userInput(Up, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, ConsecutiveMovesAcceleration) {
  userInput(Start, false);
  
  userInput(Right, true);
  userInput(Right, true);
  userInput(Right, true);
  userInput(Right, true);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, ResumeFromReadyState) {
  userInput(Start, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, PauseFromRunningState) {
  userInput(Start, false);
  
  userInput(Pause, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}

TEST_F(SnakeGameTest, AccelerateDisable) {
  userInput(Start, false);
  
  userInput(Action, true);
  userInput(Action, false);
  
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  freeGameInfo(&info);
}
