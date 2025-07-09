#include "../../../include/brickgame/frontend/cli/input.h"

#include <ncurses.h>

UserAction_t read_input(bool *hold) {
  int ch = getch();
  *hold = false;

  switch (ch) {
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_DOWN:
      return Down;
    case KEY_UP:
      return Up;  // Не используется
    case ' ':
      return Action;
    case 'p':
    case 'P':
      return Pause;
    case 'q':
    case 'Q':
      return Terminate;
    case 10:  // Enter
    case KEY_ENTER:
      return Start;
    default:
      return Up;  // Нейтральное значение
  }
}
