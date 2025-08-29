/**
 * @file app_controller.c
 * @brief Основной контроллер приложения BrickGame.
 *
 * Управляет жизненным циклом приложения, загрузкой игровых библиотек
 * и общим игровым циклом.
 */

#include "../../../include/gui/cli/app_controller.h"

#include "../../../include/gui/cli/input.h"
#include "../../../include/gui/cli/render.h"
#define _POSIX_C_SOURCE 200809L
#include <dlfcn.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Загружает игровую библиотеку и инициализирует API.
 *
 * @param game Тип игры для загрузки
 * @return GameAPI структура с функциями API или ошибкой
 */
GameAPI load_game_lib(GameType game) {
  GameAPI api = {0};
  const char* lib_name = NULL;

#ifdef __APPLE__
  const char* lib_extension = ".dylib";
#else
  const char* lib_extension = ".so";
#endif

  char lib_path[256];
  switch (game) {
    case GAME_TETRIS:
      snprintf(lib_path, sizeof(lib_path), "./libtetris%s", lib_extension);
      lib_name = lib_path;
      break;
    case GAME_SNAKE:
      snprintf(lib_path, sizeof(lib_path), "./libsnake%s", lib_extension);
      lib_name = lib_path;
      break;
    default:
      api.error = strdup("Unknown game type");
      return api;
  }

  api.lib_handle = dlopen(lib_name, RTLD_LAZY);
  if (!api.lib_handle) {
    api.error = strdup(dlerror());
    return api;
  }

  api.userInput =
      (void (*)(UserAction_t, bool))dlsym(api.lib_handle, "userInput");
  api.updateState =
      (GameInfo_t (*)(void))dlsym(api.lib_handle, "updateCurrentState");
  api.isOver = (bool (*)(void))dlsym(api.lib_handle, "isGameOver");

  if (!api.userInput || !api.updateState || !api.isOver) {
    dlclose(api.lib_handle);
    api.lib_handle = NULL;
    api.error = strdup("Failed to load required functions");
    return api;
  }

  api.valid = true;
  return api;
}

/**
 * @brief Выгружает игровую библиотеку и освобождает ресурсы.
 *
 * @param api Структура API для выгрузки
 */
void unload_game_lib(GameAPI api) {
  if (api.lib_handle) {
    dlclose(api.lib_handle);
  }
  if (api.error) {
    free(api.error);
  }
}

/**
 * @brief Инициализирует ncurses с оптимальными настройками.
 */
static void init_ncurses() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

/**
 * @brief Основной игровой цикл.
 *
 * @param api Структура API для взаимодействия с игрой
 * @param game_type Тип игры для передачи в input
 */
static void game_loop(GameAPI api, GameType game_type) {
  bool running = true;
  bool paused = false;
  bool started = false;

  while (running) {
    bool hold = false;
    UserAction_t action = read_input(&hold, game_type);

    switch (action) {
      case Pause:
        paused = !paused;
        break;
      case Start:
        started = true;
        break;
      case Terminate:
        running = false;
        continue;
      default:
        break;
    }

    api.userInput(action, hold);
    
    GameInfo_t info = api.updateState();

    if (!started) {
      renderStartScreen();
    } else if (api.isOver()) {
      flushinp();
      renderGameOverScreen();
    } else if (!paused) {
      flushinp();
      render_game(&info);
    }

    napms(info.speed);
  }
}

/**
 * @brief Запускает приложение BrickGame.
 *
 * Инициализирует ncurses, показывает меню выбора игры,
 * загружает соответствующую библиотеку и запускает игровой цикл.
 */
void run_app(void) {
  init_ncurses();

  GameType selected_game = render_game_selection();

  GameAPI api = load_game_lib(selected_game);
  if (!api.valid) {
    render_loading_error(api.error);
    endwin();
    unload_game_lib(api);
    return;
  }
  
  game_loop(api, selected_game);

  endwin();
  unload_game_lib(api);
}