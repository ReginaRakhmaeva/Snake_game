#include <gtest/gtest.h>

#include "../include/brickgame/snake/snake_fsm.hpp"
#include "../include/brickgame/snake/snake_game.hpp"

using namespace s21;

class SnakeFSMTest : public ::testing::Test {
 protected:
  SnakeGame game;
  SnakeFSM fsm = SnakeFSM(game);
};

TEST_F(SnakeFSMTest, StartFromReady) {
  EXPECT_EQ(game.GetState(), SnakeGameState::Ready);
  fsm.HandleInput(Start, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
}

TEST_F(SnakeFSMTest, PauseAndResume) {
  fsm.HandleInput(Start, false);
  fsm.HandleInput(Pause, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Paused);
  fsm.HandleInput(Pause, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
}

TEST_F(SnakeFSMTest, TerminateSetsLost) {
  fsm.HandleInput(Start, false);
  fsm.HandleInput(Terminate, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);
}

TEST_F(SnakeFSMTest, ArrowChangesDirection) {
  fsm.HandleInput(Start, false);
  game.ChangeDirection(Up);
  game.Tick();
  SnakeSegment head = SnakeGameTestHelper::GetHead(game);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);
  EXPECT_LT(after.y, head.y);
}

TEST_F(SnakeFSMTest, OppositeDirectionIgnored) {
  fsm.HandleInput(Start, false);
  game.ChangeDirection(Up);
  game.Tick();
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);

  game.ChangeDirection(Down);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);

  EXPECT_LT(after.y, before.y);
}

TEST_F(SnakeFSMTest, StartDuringPauseIgnored) {
  fsm.HandleInput(Start, false);
  fsm.HandleInput(Pause, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Paused);
  fsm.HandleInput(Start, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Paused);
}

TEST_F(SnakeFSMTest, MovementBlockedWhilePaused) {
  fsm.HandleInput(Start, false);
  SnakeSegment before = SnakeGameTestHelper::GetHead(game);

  fsm.HandleInput(Pause, false);
  game.Tick();
  SnakeSegment after = SnakeGameTestHelper::GetHead(game);

  EXPECT_EQ(before.x, after.x);
  EXPECT_EQ(before.y, after.y);
}

TEST_F(SnakeFSMTest, TerminateAfterLostIgnored) {
  fsm.HandleInput(Start, false);
  fsm.HandleInput(Terminate, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);

  fsm.HandleInput(Terminate, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);
}
TEST_F(SnakeFSMTest, StartAfterWonRestartsGame) {
  SnakeGameTestHelper::ExtendSnakeToWin(game);
  EXPECT_EQ(game.GetState(), SnakeGameState::Won);
  fsm.HandleInput(Start, true);
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
}

TEST_F(SnakeFSMTest, StartAfterLostRestartsGame) {
  fsm.HandleInput(Start, false);
  fsm.HandleInput(Terminate, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Lost);
  fsm.HandleInput(Start, false);
  EXPECT_EQ(game.GetState(), SnakeGameState::Running);
}
