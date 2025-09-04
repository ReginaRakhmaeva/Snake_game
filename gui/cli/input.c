/**
 * @brief Считывает пользовательский ввод с клавиатуры (ncurses)
 *        и преобразует его в действие игры.
 *
 * Функция обрабатывает нажатия клавиш и возвращает соответствующее
 * значение перечисления UserAction_t для дальнейшей обработки в FSM.
 * Поддерживаются стандартные клавиши управления, пауза, выход и старт.
 * Для Snake поддерживается удержание клавиш для ускорения.
 *
 * @param hold Указатель на флаг удержания клавиши.
 * @param game_type Тип игры (GAME_TETRIS или GAME_SNAKE), влияет на поведение
 * по умолчанию.
 *
 * @return UserAction_t — действие пользователя (Left, Right, Down, Up, Action,
 * Pause, Terminate, Start).
 */
#include "../../include/gui/cli/input.h"

#include <ncurses.h>

static int last_key = 0;
static int key_repeat_count = 0;
static int key_repeat_threshold = 3;

UserAction_t read_input(bool *hold, GameType game_type) {
  int ch = getch();
  *hold = false;

  if (ch == ERR) {
    key_repeat_count = 0;
    return (game_type == GAME_TETRIS) ? Up : Action;
  }

  if (ch == last_key) {
    key_repeat_count++;
  } else {
    key_repeat_count = 1;
    last_key = ch;
  }

  if (game_type == GAME_SNAKE && key_repeat_count >= key_repeat_threshold) {
    *hold = true;
  }

  switch (ch) {
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_DOWN:
      return Down;
    case KEY_UP:
      return Up;
    case ' ':
      return Action;
    case 'p':
    case 'P':
      return Pause;
    case 'q':
    case 'Q':
      return Terminate;
    case 10:
    case KEY_ENTER:
      return Start;
    default:
      return (game_type == GAME_TETRIS) ? Up : Action;
  }
}
