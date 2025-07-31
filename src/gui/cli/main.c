#include <dlfcn.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "../../../include/brickgame/snake/snake_api.h"
#include "../../../include/brickgame/tetris/game.h"
#include "../../../include/gui/cli/input.h"
#include "../../../include/gui/cli/render.h"
#include "brickgame/common/types.h"

typedef void (*userInput_t)(int, bool);
typedef GameInfo_t (*updateCurrentState_t)(void);
typedef bool (*isGameOver_t)(void);
// при паузе змейка не запускается (вроде починила)
// написать запоминание счета
// проверить статики в бэке, насколько надежно
// двигается вправо хотелось бы налево ну или хотябы ближе к другой стороне
int main(void) {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  // Вызбор игры
  mvprintw(5, 10, "Select a game:");
  mvprintw(7, 12, "1 - Tetris");
  mvprintw(8, 12, "2 - Snake");
  refresh();
  int ch = 0;
  while (ch != '1' && ch != '2') ch = getch();

  GameType current_game = GAME_TETRIS;  // по умолчанию

  // При выборе игры
  if (ch == '1') {
    current_game = GAME_TETRIS;
  } else if (ch == '2') {
    current_game = GAME_SNAKE;
  }

  void *lib = NULL;
  if (current_game == GAME_TETRIS)
    lib = dlopen("./libtetris.so", RTLD_LAZY);
  else if (current_game == GAME_SNAKE)
    lib = dlopen("./libsnake.so", RTLD_LAZY);
  if (!lib) {
    endwin();
    printf("Loading error: %s\n", dlerror());
    return 1;
  }

  userInput_t userInput = (userInput_t)dlsym(lib, "userInput");
  updateCurrentState_t updateCurrentState =
      (updateCurrentState_t)dlsym(lib, "updateCurrentState");
  isGameOver_t isGameOver = (isGameOver_t)dlsym(lib, "isGameOver");

  bool running = true;
  bool paused = false;
  bool start = false;

  while (running) {
    bool hold = false;
    UserAction_t action = read_input(&hold, current_game);
    if (action == Pause) paused = !paused;
    if (action == Start) start = true;
    if (action == Terminate) running = false;

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
  dlclose(lib);
  return 0;
}