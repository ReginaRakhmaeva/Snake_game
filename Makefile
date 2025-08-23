# === Определение ОС ===
UNAME_S := $(shell uname -s)

# === Компиляторы и флаги ===
CC = gcc
CXX = g++
CFLAGS = -fPIC -Wall -Wextra -std=c99 -g -O0 -Iinclude -fvisibility=hidden
CXXFLAGS = -fPIC -Wall -Wextra -std=c++20 -g -O0 -Iinclude -fvisibility=hidden

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
ifeq ($(OS),Windows_NT)
    LDFLAGS = 
    SHARED_EXT = .dll
    SHARED_FLAGS = -shared
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
all: $(LIBTETRIS) $(LIBSNAKE) brickgame_cli brickgame_desktop

$(LIBTETRIS): $(TETRIS_SRC)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -o $@ $(TETRIS_SRC)

$(LIBSNAKE): $(SNAKE_SRC)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $(SNAKE_SRC)

brickgame_cli: $(CLI_SRC)
	$(CC) $(CFLAGS) -o $@ $(CLI_SRC) $(LDFLAGS)

brickgame_desktop: $(LIBTETRIS) $(LIBSNAKE)
	@echo "=== Building Qt frontend ==="
	@mkdir -p build_qt
	cd build_qt && cmake "$$(pwd)/.." && make
	@cp build_qt/brickgame_desktop . || true

install: all
	@echo "Installing CLI to /usr/local/bin..."
	@mkdir -p /usr/local/bin
	cp brickgame_cli /usr/local/bin/

uninstall:
	@echo "Uninstalling..."
	rm -f /usr/local/bin/brickgame_cli

clean:
	@echo "=== Cleaning all build artifacts ==="
	# Удаляем исполняемые файлы
	rm -f $(LIBTETRIS) $(LIBSNAKE) brickgame_cli brickgame_desktop
	rm -f brickgame_desktop_autogen
	
	# Удаляем отладочные символы
	rm -rf *.dSYM
	
	# Удаляем объектные файлы и временные файлы
	rm -f *.o *.out *.a *.gcda *.gcno *.gcov
	rm -f *.so *.dylib *.dll
	rm -f *.exe *.app
	
	# Удаляем логи и отладочные файлы
	rm -f debug*.log debug*.txt
	rm -f *.log
	rm -f snake_highscore.txt tetris_highscore.txt
	
	# Удаляем директории сборки (но сохраняем CMakeLists.txt)
	rm -rf $(QT_BUILD_DIR)
	rm -rf build_qt
	rm -rf test_snake_bin
	rm -rf CMakeFiles
	rm -f CMakeCache.txt
	rm -f cmake_install.cmake
	rm -f Makefile.cmake
	
	# Удаляем файлы Qt
	rm -rf *.moc
	rm -rf moc_*.cpp
	rm -rf ui_*.h
	rm -rf qrc_*.cpp
	rm -rf *_autogen
	
	# Удаляем временные файлы системы
	rm -rf .DS_Store
	rm -rf Thumbs.db
	rm -f *~
	rm -f .#*
	rm -f #*#
	
	# Удаляем файлы IDE
	rm -rf .vscode
	rm -rf .idea
	rm -f *.swp *.swo
	rm -f *~
	
	# Удаляем архивы
	rm -f brickgame-*.tar.gz
	rm -f brickgame-*.zip
	
	@echo "=== Clean completed ==="

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

