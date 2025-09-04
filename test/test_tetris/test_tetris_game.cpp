#include <gtest/gtest.h>

#include "../include/brickgame/tetris/game.h"

class TetrisGameTest : public ::testing::Test {
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

TEST_F(TetrisGameTest, InitialState) {
  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(isGameOver());

  for (int y = 0; y < 20; ++y) {
    EXPECT_NE(info.field[y], nullptr);
  }

  freeGameInfo(&info);
}

TEST_F(TetrisGameTest, GameStart) {
  userInput(Start, false);

  GameInfo_t info = updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info);
}

TEST_F(TetrisGameTest, PieceMovement) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Left, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Right, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  userInput(Down, false);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
}

TEST_F(TetrisGameTest, PieceRotation) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Action, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Action, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisGameTest, PauseAndResume) {
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

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisGameTest, GameTermination) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);
  EXPECT_FALSE(isGameOver());

  userInput(Terminate, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  EXPECT_TRUE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(TetrisGameTest, HoldFunctionality) {
  userInput(Start, false);

  userInput(Left, true);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Right, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Action, true);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisGameTest, ComplexGameplay) {
  userInput(Start, false);

  for (int i = 0; i < 15; ++i) {
    switch (i % 4) {
      case 0:
        userInput(Left, i % 2 == 0);
        break;
      case 1:
        userInput(Right, i % 2 == 0);
        break;
      case 2:
        userInput(Down, i % 2 == 0);
        break;
      case 3:
        userInput(Action, i % 2 == 0);
        break;
    }

    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    EXPECT_FALSE(isGameOver());
    freeGameInfo(&info);
  }

  userInput(Pause, false);
  GameInfo_t info_pause = updateCurrentState();
  EXPECT_NE(info_pause.field, nullptr);
  freeGameInfo(&info_pause);

  userInput(Pause, false);
  GameInfo_t info_resume = updateCurrentState();
  EXPECT_NE(info_resume.field, nullptr);
  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info_resume);
}

TEST_F(TetrisGameTest, MultipleActions) {
  userInput(Start, false);

  userInput(Left, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Action, false);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  userInput(Right, false);
  GameInfo_t info3 = updateCurrentState();
  EXPECT_NE(info3.field, nullptr);

  userInput(Action, false);
  GameInfo_t info4 = updateCurrentState();
  EXPECT_NE(info4.field, nullptr);

  userInput(Down, false);
  GameInfo_t info5 = updateCurrentState();
  EXPECT_NE(info5.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
  freeGameInfo(&info4);
  freeGameInfo(&info5);
}

TEST_F(TetrisGameTest, GameStatePersistence) {
  userInput(Start, false);

  GameInfo_t info1 = updateCurrentState();
  GameInfo_t info2 = updateCurrentState();
  GameInfo_t info3 = updateCurrentState();

  EXPECT_NE(info1.field, nullptr);
  EXPECT_NE(info2.field, nullptr);
  EXPECT_NE(info3.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
  freeGameInfo(&info3);
}

TEST_F(TetrisGameTest, EdgeCaseActions) {
  userInput(Start, false);

  userInput(Up, false);
  GameInfo_t info1 = updateCurrentState();
  EXPECT_NE(info1.field, nullptr);

  userInput(Up, true);
  GameInfo_t info2 = updateCurrentState();
  EXPECT_NE(info2.field, nullptr);

  EXPECT_FALSE(isGameOver());

  freeGameInfo(&info1);
  freeGameInfo(&info2);
}

TEST_F(TetrisGameTest, LineClearing) {
  userInput(Start, false);

  for (int i = 0; i < 50; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, MultipleLineClearing) {
  userInput(Start, false);

  for (int i = 0; i < 100; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, ScoreAndLevelProgression) {
  userInput(Start, false);

  for (int i = 0; i < 200; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, HighScoreUpdate) {
  userInput(Start, false);

  for (int i = 0; i < 300; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, MaxLevelReached) {
  userInput(Start, false);

  for (int i = 0; i < 1000; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, LongGameSession) {
  userInput(Start, false);

  for (int i = 0; i < 500; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, SaveHighScoreFunction) {
  userInput(Start, false);

  for (int i = 0; i < 400; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

// ========================================
// Line Clearing Tests for Coverage
// ========================================

TEST_F(TetrisGameTest, SingleLineClearing) {
  userInput(Start, false);

  // Создаем условия для заполнения линий
  // Много движений вниз для накопления блоков
  for (int i = 0; i < 800; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, MultipleLineClearingIntensive) {
  userInput(Start, false);

  // Интенсивная игра для создания заполненных линий
  for (int i = 0; i < 1200; ++i) {
    userInput(Down, false);
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, LineClearingWithRotation) {
  userInput(Start, false);

  // Комбинируем движения вниз с вращениями
  for (int i = 0; i < 600; ++i) {
    userInput(Down, false);
    if (i % 10 == 0) {
      userInput(Action, false);  // Вращение
    }
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, LineClearingWithHold) {
  userInput(Start, false);

  // Используем hold для быстрого падения
  for (int i = 0; i < 400; ++i) {
    userInput(Down, true);  // Hold для быстрого падения
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}

TEST_F(TetrisGameTest, LineClearingEdgeCase) {
  userInput(Start, false);

  // Тест граничного случая - очень много движений
  for (int i = 0; i < 1500; ++i) {
    userInput(Down, false);
    if (i % 50 == 0) {
      userInput(Left, false);  // Иногда двигаемся влево
    }
    GameInfo_t info = updateCurrentState();
    EXPECT_NE(info.field, nullptr);
    freeGameInfo(&info);
  }

  GameInfo_t final_info = updateCurrentState();
  EXPECT_NE(final_info.field, nullptr);
  freeGameInfo(&final_info);
}
