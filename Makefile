# === Определение ОС ===
UNAME_S := $(shell uname -s)

# === Компиляторы и флаги ===
CC = gcc
CXX = g++
CFLAGS = -fPIC -Wall -Wextra -std=c99 -g -O0 -Iinclude
CXXFLAGS = -fPIC -Wall -Wextra -std=c++20 -g -O0 -Iinclude

# === Настройки под ОС ===
ifeq ($(UNAME_S),Linux)
    LDFLAGS = -ldl -lncurses
    SHARED_EXT = .so
    SHARED_FLAGS = -shared
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -lncurses
    SHARED_EXT = .dylib
    SHARED_FLAGS = -dynamiclib
endif

# === Исходники ===
TETRIS_SRC = src/brickgame/tetris/backend.c \
             src/brickgame/tetris/fsm.c \
             src/brickgame/tetris/game.c

SNAKE_SRC  = src/brickgame/snake/snake_api.cpp \
             src/brickgame/snake/snake_fsm.cpp \
             src/brickgame/snake/snake_game.cpp

CLI_SRC    = src/gui/cli/main.c \
             src/gui/cli/input.c \
             src/gui/cli/render.c \
             src/gui/cli/app_controller.c

# === Библиотеки ===
LIBTETRIS = libtetris$(SHARED_EXT)
LIBSNAKE  = libsnake$(SHARED_EXT)

# === Qt проект ===
QT_BUILD_DIR = build_qt

# === Цели ===
all: $(LIBTETRIS) $(LIBSNAKE) brickgame_cli snake_qt

$(LIBTETRIS): $(TETRIS_SRC)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -o $@ $(TETRIS_SRC)

$(LIBSNAKE): $(SNAKE_SRC)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $(SNAKE_SRC)

brickgame_cli: $(CLI_SRC)
	$(CC) $(CFLAGS) -o $@ $(CLI_SRC) $(LDFLAGS)

snake_qt:
	@echo "=== Building Qt frontend ==="
	@mkdir -p $(QT_BUILD_DIR) 
	cd $(QT_BUILD_DIR) && cmake .. && $(MAKE)
	

install: all
	@echo "Installing CLI to /usr/local/bin..."
	@mkdir -p /usr/local/bin
	cp brickgame_cli /usr/local/bin/

uninstall:
	@echo "Uninstalling..."
	rm -f /usr/local/bin/brickgame_cli

clean:
	rm -f $(LIBTETRIS) $(LIBSNAKE) brickgame_cli
	rm -f *.o *.out *.a *.gcda *.gcno
	rm -f debug.log debug1.log
	rm -f snake_highscore.txt tetris_highscore.txt
	rm -rf $(QT_BUILD_DIR)
	rm -rf snake_qt test_snake_bin

dist: clean
	@echo "Creating distribution archive..."
	tar -czf brickgame-$(shell date +%Y%m%d).tar.gz *

# === Тесты ===
TEST_SNAKE_SRC = test_snake/test_snake_game.cpp \
				test_snake/test_main.cpp \
                 test_snake/test_snake_fsm.cpp

TEST_SNAKE_BIN = test_snake_bin


test_snake: $(LIBSNAKE) $(TEST_SNAKE_SRC)
	$(CXX) $(CXXFLAGS) -o test_snake_bin $(TEST_SNAKE_SRC) -L. -lsnake -lgtest -lpthread
	LD_LIBRARY_PATH=. ./test_snake_bin


.PHONY: all clean install uninstall dist snake_qt test_snake

