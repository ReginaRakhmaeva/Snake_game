/**
 * @file render.c
 * @brief Интерфейсные функции отрисовки для игры Tetris (CLI).
 *
 * Содержит функции отрисовки основного игрового экрана,
 * стартового экрана, границ, поля, информации и состояния паузы.
 */

#include "../../../include/gui/cli/render.h"

#include <ncurses.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIELD_OFFSET_X 2
#define FIELD_OFFSET_Y 1
#define SCREEN_CENTER_X 20
#define SCREEN_CENTER_Y 10

/**
 * @brief Очищает экран и рисует границы поля.
 */
void drawFieldBorders() {
  for (int y = 0; y <= FIELD_HEIGHT; ++y) {
    mvprintw(FIELD_OFFSET_Y + y, FIELD_OFFSET_X - 1, "|");
    mvprintw(FIELD_OFFSET_Y + y, FIELD_OFFSET_X + FIELD_WIDTH * 2, "|");
  }
  for (int x = 0; x <= FIELD_WIDTH * 2; ++x) {
    mvprintw(FIELD_OFFSET_Y - 1, FIELD_OFFSET_X + x, "-");
    mvprintw(FIELD_OFFSET_Y + FIELD_HEIGHT, FIELD_OFFSET_X + x, "-");
  }
  mvprintw(FIELD_OFFSET_Y - 1, FIELD_OFFSET_X - 1, "+");
  mvprintw(FIELD_OFFSET_Y - 1, FIELD_OFFSET_X + FIELD_WIDTH * 2, "+");
  mvprintw(FIELD_OFFSET_Y + FIELD_HEIGHT, FIELD_OFFSET_X - 1, "+");
  mvprintw(FIELD_OFFSET_Y + FIELD_HEIGHT, FIELD_OFFSET_X + FIELD_WIDTH * 2,
           "+");
}

/**
 * @brief Отрисовывает игровое поле и информацию.
 *
 * @param info Указатель на структуру GameInfo_t
 */
void render_game(const GameInfo_t *info) {
  mvprintw(0, 0, "field ptr: %p", (void *)info->field);
  mvprintw(1, 0, "score: %d", info->score);

  clear();
  drawFieldBorders();

  // Поле
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      mvprintw(FIELD_OFFSET_Y + y, FIELD_OFFSET_X + x * 2, "%s",
               info->field && info->field[y][x] ? "[]" : "  ");
    }
  }

  // Next фигура (только для тетриса)
  if (info->next) {
    mvprintw(1, 25, "Next:");
    for (int y = 0; y < 4; ++y) {
      for (int x = 0; x < 4; ++x) {
        mvprintw(2 + y, 25 + x * 2, "%s", info->next[y][x] ? "[]" : "  ");
      }
    }
  }

  // Статистика
  mvprintw(8, 25, "Score: %d", info->score);
  mvprintw(9, 25, "High:  %d", info->high_score);
  mvprintw(10, 25, "Level: %d", info->level);

  refresh();
}

/**
 * @brief Отображает стартовый экран с правилами управления.
 */
void renderStartScreen() {
  clear();

  mvprintw(SCREEN_CENTER_Y - 5, SCREEN_CENTER_X - 4, "BRICKGAME");
  mvprintw(SCREEN_CENTER_Y - 2, SCREEN_CENTER_X - 10, "Press ENTER to Start");
  mvprintw(SCREEN_CENTER_Y - 1, SCREEN_CENTER_X - 6, "or Q to Quit");

  mvprintw(SCREEN_CENTER_Y + 1, SCREEN_CENTER_X - 4, "== RULES ==");
  mvprintw(SCREEN_CENTER_Y + 2, SCREEN_CENTER_X - 10, "Arrow Keys : Move");
  mvprintw(SCREEN_CENTER_Y + 3, SCREEN_CENTER_X - 10,
           "Space/Action: Rotate/Speed up");
  mvprintw(SCREEN_CENTER_Y + 4, SCREEN_CENTER_X - 10, "P          : Pause");
  mvprintw(SCREEN_CENTER_Y + 5, SCREEN_CENTER_X - 10, "Q          : Quit");

  refresh();
}
void renderGameOverScreen() {
  clear();
  mvprintw(SCREEN_CENTER_Y - 5, SCREEN_CENTER_X - 8, "=== GAME OVER ===");
  mvprintw(SCREEN_CENTER_Y - 2, SCREEN_CENTER_X - 11, "Press ENTER to restart");
  mvprintw(SCREEN_CENTER_Y - 1, SCREEN_CENTER_X - 8, "Press Q to exit");
  refresh();
}
void renderGameWonScreen() {
  clear();
  mvprintw(SCREEN_CENTER_Y - 5, SCREEN_CENTER_X - 8, "=== YOU WON! ===");
  mvprintw(SCREEN_CENTER_Y - 2, SCREEN_CENTER_X - 11, "Press ENTER to restart");
  mvprintw(SCREEN_CENTER_Y - 1, SCREEN_CENTER_X - 8, "Press Q to exit");
  refresh();
}

/**
 * @brief Отображает экран выбора игры и возвращает выбранную игру.
 *
 * @return GameType - выбранная игра
 */
GameType render_game_selection() {
  clear();
  mvprintw(SCREEN_CENTER_Y - 5, SCREEN_CENTER_X - 4,
           "===== BRICKGAME COLLECTION =====");
  mvprintw(7, 12, "Select a game:");
  mvprintw(9, 14, "1 - Tetris");
  mvprintw(10, 14, "2 - Snake");
  mvprintw(12, 12, "Press 1 or 2 to select");
  refresh();

  int ch = 0;
  while (ch != '1' && ch != '2') {
    ch = getch();
  }

  return (ch == '1') ? GAME_TETRIS : GAME_SNAKE;
}

/**
 * @brief Отображает ошибку загрузки библиотеки.
 *
 * @param error - текст ошибки
 */
void render_loading_error(const char *error) {
  clear();
  mvprintw(SCREEN_CENTER_Y - 2, SCREEN_CENTER_X - 10, "=== LOADING ERROR ===");
  mvprintw(SCREEN_CENTER_Y, SCREEN_CENTER_X - 15, "Error: %s", error);
  mvprintw(SCREEN_CENTER_Y + 2, SCREEN_CENTER_X - 12, "Press any key to exit");
  refresh();
  getch();
}
