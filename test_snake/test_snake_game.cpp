#include <gtest/gtest.h>

#include "../include/brickgame/snake/snake_fsm.hpp"
#include "../include/brickgame/snake/snake_game.hpp"

using namespace s21;

class SnakeGameTest : public ::testing::Test {
 protected:
  SnakeGame game;
};

TEST_F(SnakeGameTest, StartsInReadyState) {
  EXPECT_EQ(game.GetState(), SnakeGameState::Ready);
}

TEST_F(SnakeGameTest, ResumeInitializesGame) {
  game.Resume();
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
}

TEST_F(SnakeGameTest, SnakeMovesAndEatsApple) {
  game.Resume();
  SnakeSegment head = SnakeGameTestHelper::GetHead(game);
  SnakeGameTestHelper::ForceAppleAt(game, head.x + 1, head.y);
  game.Tick();

  GameInfo_t info = game.GetGameInfo();
  EXPECT_GT(info.score, 0);
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
  game.FreeGameInfo(info);
}

TEST_F(SnakeGameTest, HitsWallAndLoses) {
  game.Resume();
  SnakeGameTestHelper::MoveSnakeToWall(game);
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);
}

TEST_F(SnakeGameTest, WinsIfSnakeIsMaxLength) {
  game.Resume();
  SnakeGameTestHelper::ExtendSnakeToWin(game);
  EXPECT_EQ(game.GetState(), SnakeGameState::Won);
}

TEST_F(SnakeGameTest, InfoMemoryIsAllocatedAndFreed) {
  game.Resume();
  GameInfo_t info = game.GetGameInfo();
  ASSERT_NE(info.field, nullptr);
  for (int y = 0; y < kGameHeight; ++y) ASSERT_NE(info.field[y], nullptr);
  game.FreeGameInfo(info);
  EXPECT_EQ(info.field, nullptr);
}

TEST_F(SnakeGameTest, GameRejectsOppositeDirection) {
  game.Resume();
  game.ChangeDirection(Left);
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_EQ(after.y, before.y);
  EXPECT_GT(after.x, before.x);
}

TEST_F(SnakeGameTest, AppleNotPlacedOnSnake) {
  game.Resume();
  GameInfo_t info = game.GetGameInfo();
  int apple_count = 0;
  for (int y = 0; y < kGameHeight; ++y)
    for (int x = 0; x < kGameWidth; ++x)
      if (info.field[y][x] == static_cast<int>(CellType::Apple)) ++apple_count;

  EXPECT_EQ(apple_count, 1);
  game.FreeGameInfo(info);
}

TEST_F(SnakeGameTest, ApplePlacedCorrectlyWhenOneCellLeft) {
  game.Resume();

  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      SnakeGameTestHelper::CellAt(game, y, x) =
          static_cast<int>(CellType::Snake);
    }
  }

  int free_x = 0;
  int free_y = 0;
  SnakeGameTestHelper::CellAt(game, free_y, free_x) =
      static_cast<int>(CellType::Empty);

  SnakeGameTestHelper::SnakeSegments(game).clear();
  for (int y = 0; y < kGameHeight; ++y) {
    for (int x = 0; x < kGameWidth; ++x) {
      if (!(x == free_x && y == free_y)) {
        SnakeGameTestHelper::SnakeSegments(game).push_back({x, y});
      }
    }
  }

  SnakeGameTestHelper::SetLength(
      game, static_cast<int>(SnakeGameTestHelper::SnakeSegments(game).size()));
  SnakeGameTestHelper::SetState(game, SnakeGameState::Running);

  SnakeGameTestHelper::CallPlaceApple(game);

  EXPECT_EQ(SnakeGameTestHelper::GetAppleX(game), free_x);
  EXPECT_EQ(SnakeGameTestHelper::GetAppleY(game), free_y);

  for (const auto& segment : SnakeGameTestHelper::SnakeSegments(game)) {
    EXPECT_FALSE(segment.x == SnakeGameTestHelper::GetAppleX(game) &&
                 segment.y == SnakeGameTestHelper::GetAppleY(game));
  }
}

TEST_F(SnakeGameTest, UpdateDoesNothingWhenLost) {
  game.Resume();
  SnakeGameTestHelper::MoveSnakeToWall(game);
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_EQ(after.x, before.x);
  EXPECT_EQ(after.y, before.y);
}

TEST_F(SnakeGameTest, CheckCollisionDetectsSnake) {
  game.Resume();
  SnakeSegment head = SnakeGameTestHelper::GetHead(game);
  EXPECT_TRUE(SnakeGameTestHelper::TestCollision(game, head.x, head.y));
}

TEST_F(SnakeGameTest, ResetResetsStateAndField) {
  game.Resume();
  SnakeGameTestHelper::ExtendSnakeToWin(game);
  EXPECT_EQ(game.GetState(), SnakeGameState::Won);

  game.Reset();
  EXPECT_EQ(game.GetState(), SnakeGameState::Ready);
  GameInfo_t info = game.GetGameInfo();
  for (int y = 0; y < kGameHeight; ++y)
    for (int x = 0; x < kGameWidth; ++x)
      EXPECT_EQ(info.field[y][x], static_cast<int>(CellType::Empty));
  game.FreeGameInfo(info);
}

TEST_F(SnakeGameTest, AccelerationFlagDoesNotAffectLogicDirectly) {
  game.Resume();
  game.Accelerate(true);
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_GT(after.x, before.x);
}
TEST_F(SnakeGameTest, ChangeDirectionIgnoredInReady) {
  EXPECT_EQ(game.GetState(), SnakeGameState::Ready);
  game.ChangeDirection(Down);
  game.Resume();
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_GT(after.x, before.x);
}

TEST_F(SnakeGameTest, PauseAndResumeWorkCorrectly) {
  game.Resume();
  game.Pause();
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_EQ(before.x, after.x);
  EXPECT_EQ(before.y, after.y);

  game.Resume();
  game.Tick();
  SnakeSegment moved = SnakeGameTestHelper::GetHead(game);
  EXPECT_NE(moved.x, after.x);
}

TEST_F(SnakeGameTest, TerminateForcesLostState) {
  game.Resume();
  game.Terminate();
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);
}
