#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>

#include "../../../include/gui/cli/input.h"
#include "../../../include/gui/cli/render.h"
#include "../../../include/brickgame/tetris/game.h"
#include "../../../include/brickgame/snake/snake_api.h"
#include "brickgame/common/types.h"


typedef void (*userInput_t)(int, bool);
typedef GameInfo_t (*updateCurrentState_t)(void);
typedef bool (*isGameOver_t)(void);

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

    void *lib = NULL;
    if (ch == '1') lib = dlopen("./libtetris.so", RTLD_LAZY);
    else if (ch == '2') lib = dlopen("./libsnake.so", RTLD_LAZY);
    if (!lib) {
        endwin();
        printf("Loading error: %s\n", dlerror());
        return 1;
    }

    userInput_t userInput = (userInput_t)dlsym(lib, "userInput");
    updateCurrentState_t updateCurrentState = (updateCurrentState_t)dlsym(lib, "updateCurrentState");
    isGameOver_t isGameOver = (isGameOver_t)dlsym(lib, "isGameOver");

    // Добавь эти строки для отладки:
    FILE *log = fopen("debug.log", "a");
    fprintf(log, "userInput addr: %p\n", (void*)userInput);
    fprintf(log, "updateCurrentState addr: %p\n", (void*)updateCurrentState);
    fprintf(log, "isGameOver addr: %p\n", (void*)isGameOver);
    fclose(log);

    bool running = true;
    bool paused = false;
    bool start = false;

    while (running) {
        bool hold = false;
        int action = read_input(&hold);
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