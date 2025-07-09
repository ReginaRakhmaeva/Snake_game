#include "../../../include/brickgame/tetris/backend.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "../../../include/brickgame/tetris/fsm.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIGURE_SIZE 4

typedef struct {
  int shape[FIGURE_SIZE][FIGURE_SIZE];
  int x, y;
} Tetromino;

static const int FIGURES[7][FIGURE_SIZE][FIGURE_SIZE] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}};

static int field[FIELD_HEIGHT][FIELD_WIDTH];
static Tetromino current_piece;
static Tetromino next_piece;
static GameInfo_t info;

static void rotate_clockwise(int src[FIGURE_SIZE][FIGURE_SIZE],
                             int dst[FIGURE_SIZE][FIGURE_SIZE]) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      dst[x][FIGURE_SIZE - 1 - y] = src[y][x];
    }
  }
}

static void spawn_piece(Tetromino *dst) {
  int id = rand() % 7;
  memcpy(dst->shape, FIGURES[id], sizeof(dst->shape));
  dst->x = 3;
  dst->y = -2;
}

static void clear_lines() {
  int lines_cleared = 0;
  for (int y = FIELD_HEIGHT - 1; y >= 0; --y) {
    int full = 1;
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (!field[y][x]) {
        full = 0;
        break;
      }
    }

    if (full) {
      lines_cleared++;
      for (int ty = y; ty > 0; --ty) {
        memcpy(field[ty], field[ty - 1], sizeof(field[0]));
      }
      memset(field[0], 0, sizeof(field[0]));
      y++;
    }
  }

  if (lines_cleared > 0) {
    switch (lines_cleared) {
      case 1:
        info.score += 100;
        break;
      case 2:
        info.score += 300;
        break;
      case 3:
        info.score += 700;
        break;
      case 4:
        info.score += 1500;
        break;
    }

    if (info.score > info.high_score) {
      info.high_score = info.score;
      save_high_score(info.high_score);
    }

    int new_level = 1 + info.score / 600;
    if (new_level > 10) new_level = 10;
    if (new_level != info.level) {
      info.level = new_level;
    }
    info.speed = get_level_speed(info.level);
  }
}
void save_high_score(int high_score) {
  FILE *file = fopen("tetris_highscore.txt", "w");
  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}
static int load_high_score() {
  FILE *file = fopen(SCORE_FILE, "r");
  int high_score = 0;
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

GameInfo_t backend_init_game(void) {
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      field[y][x] = 0;
    }
  }
  spawn_piece(&current_piece);
  spawn_piece(&next_piece);

  info.field = (int **)malloc(FIELD_HEIGHT * sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    info.field[i] = field[i];
  }

  info.next = (int **)malloc(FIGURE_SIZE * sizeof(int *));
  for (int i = 0; i < FIGURE_SIZE; ++i) {
    info.next[i] = next_piece.shape[i];
  }

  info.score = 0;
  info.high_score = load_high_score();
  info.level = 1;
  info.speed = get_level_speed(info.level);
  info.pause = 0;

  return info;
}

static int check_collision(int dx, int dy) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int nx = current_piece.x + x + dx;
        int ny = current_piece.y + y + dy;

        if (nx < 0 || nx >= FIELD_WIDTH || ny >= FIELD_HEIGHT) return 1;
        if (ny >= 0 && field[ny][nx]) return 1;
      }
    }
  }
  return 0;
}
static int check_spawn_failure() {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fy >= 0 && field[fy][fx]) return 1;
      }
    }
  }
  return 0;
}

BackendStatus backend_update_physics(GameInfo_t *info_ptr) {
  (void)info_ptr;
  if (!check_collision(0, 1)) {
    current_piece.y += 1;
    info.score += 0;
    return BACKEND_OK;
  } else {
    return backend_fix_piece();
  }
}

static int try_rotate() {
  int rotated[FIGURE_SIZE][FIGURE_SIZE];
  rotate_clockwise(current_piece.shape, rotated);

  const int offsets[] = {0, -1, 1, -2, 2};
  for (size_t i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
    int dx = offsets[i];
    Tetromino temp = current_piece;
    temp.x += dx;
    memcpy(temp.shape, rotated, sizeof(rotated));

    int collision = 0;
    for (int y = 0; y < FIGURE_SIZE; ++y) {
      for (int x = 0; x < FIGURE_SIZE; ++x) {
        if (temp.shape[y][x]) {
          int fx = temp.x + x;
          int fy = temp.y + y;
          if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT) {
            collision = 1;
            break;
          }
          if (fy >= 0 && field[fy][fx]) {
            collision = 1;
            break;
          }
        }
      }
      if (collision) break;
    }

    if (!collision) {
      current_piece = temp;
      return 1;
    }
  }

  return 0;
}

BackendStatus backend_handle_input(UserAction_t action, bool hold) {
  switch (action) {
    case Left:
      if (!check_collision(-1, 0)) current_piece.x -= 1;
      break;
    case Right:
      if (!check_collision(1, 0)) current_piece.x += 1;
      break;
    case Down:
      if (hold) {
        while (!check_collision(0, 1)) {
          current_piece.y += 1;
        }
        return backend_fix_piece();
      } else {
        if (!check_collision(0, 1)) current_piece.y += 1;
      }
      break;
    case Action:
      try_rotate();
      break;
    default:
      break;
  }
  return BACKEND_OK;
}

void backend_overlay_piece(GameInfo_t *info_ptr) {
  static int temp_field[FIELD_HEIGHT][FIELD_WIDTH];

  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      temp_field[y][x] = field[y][x];
    }
  }

  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT) {
          temp_field[fy][fx] = 1;
        }
      }
    }
  }

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    info_ptr->field[i] = temp_field[i];
  }
}

BackendStatus backend_fix_piece(void) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      if (current_piece.shape[y][x]) {
        int fx = current_piece.x + x;
        int fy = current_piece.y + y;
        if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH) {
          field[fy][fx] = 1;
        }
      }
    }
  }

  clear_lines();

  current_piece = next_piece;
  spawn_piece(&next_piece);

  if (check_spawn_failure()) {
    return BACKEND_GAME_OVER;
  }
  return BACKEND_OK;
}
// BackendStatus backend_fix_piece(void) {
//   // Проверка: если хотя бы один блок фигуры выйдет выше поля — конец игры
//   for (int y = 0; y < FIGURE_SIZE; ++y) {
//     for (int x = 0; x < FIGURE_SIZE; ++x) {
//       if (current_piece.shape[y][x]) {
//         int fy = current_piece.y + y;
//         if (fy < 0) {
//           return BACKEND_GAME_OVER;
//         }
//       }
//     }
//   }

//   // Фиксируем фигуру
//   for (int y = 0; y < FIGURE_SIZE; ++y) {
//     for (int x = 0; x < FIGURE_SIZE; ++x) {
//       if (current_piece.shape[y][x]) {
//         int fx = current_piece.x + x;
//         int fy = current_piece.y + y;
//         if (fy >= 0 && fy < FIELD_HEIGHT && fx >= 0 && fx < FIELD_WIDTH) {
//           field[fy][fx] = 1;
//         }
//       }
//     }
//   }

//   clear_lines();

//   current_piece = next_piece;
//   spawn_piece(&next_piece);

//   if (check_spawn_failure()) {
//     return BACKEND_GAME_OVER;
//   }

//   return BACKEND_OK;
// }


GameInfo_t backend_get_info(void) { return info; }

int get_level_speed(int level) {
    int speed = 600 - (level - 1) * 60;
    if (speed < 80) speed = 80;
    return speed;
}
