#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

#include "../../../include/brickgame/frontend/cli/input.h"
#include "../../../include/brickgame/frontend/cli/render.h"
#include "../../../include/brickgame/tetris/game.h"

int main(void) {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  bool running = true;
  bool paused = false;
  bool start = false;

  while (running) {
    bool hold = false;
    UserAction_t action = read_input(&hold);
    if (action == Pause) {
      paused = !paused;
    }
    if (action == Start) {
      start = true;
    }
    if (action == Terminate) {
      running = false;
    }

    userInput(action, hold);

    GameInfo_t info = updateCurrentState();

    if (!start) {
      renderStartScreen();
    } else if (isGameOver()) {
      flushinp();
      renderGameOverScreen();

    } else if (!paused) {
      flushinp();
      render_game(&info);
    }
    napms(info.speed);
  }

  endwin();
  return 0;
}
