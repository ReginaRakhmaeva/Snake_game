# === Компиляторы и флаги ===
CC = gcc
CXX = g++
CFLAGS = -fPIC -Wall -Wextra -std=c99 -g -O0 -Iinclude
CXXFLAGS = -fPIC -Wall -Wextra -std=c++20 -g -O0 -Iinclude

LDFLAGS = -ldl -lncurses

TETRIS_SRC = src/brickgame/tetris/backend.c src/brickgame/tetris/fsm.c src/brickgame/tetris/game.c
SNAKE_SRC = src/brickgame/snake/snake_api.cpp src/brickgame/snake/snake_fsm.cpp src/brickgame/snake/snake_game.cpp

LIBTETRIS = libtetris.so
LIBSNAKE = libsnake.so

CLI_SRC = src/gui/cli/main.c src/gui/cli/input.c src/gui/cli/render.c

all: $(LIBTETRIS) $(LIBSNAKE) brickgame_cli

$(LIBTETRIS): $(TETRIS_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $(TETRIS_SRC)

$(LIBSNAKE): $(SNAKE_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(SNAKE_SRC)

brickgame_cli: $(CLI_SRC)
	$(CC) $(CFLAGS) -o $@ $(CLI_SRC) $(LDFLAGS)

clean:
#	rm -rf $(BUILD_DIR)
	rm -f $(LIBTETRIS) $(LIBSNAKE) $(BRICKGAME_CLI)
	rm -f snake_cli tetris_cli
	rm -f debug.log debug1.log
	rm -f snake_highscore.txt tetris_highscore.txt
	rm -f *.o *.out *.a
	rm -f brickgame_cli
