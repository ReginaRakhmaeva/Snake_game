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
	rm -f test/*.gcno test/*.gcda
	rm -f test/test_snake_bin test/test_tetris_bin
	rm -f test/coverage.info test/coverage_filtered.info
	rm -rf test/coverage_report test/lcov_report
	rm -rf coverage_report
	rm -rf lcov_report
	rm -f coverage.info
	rm -f coverage_filtered.info
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
TEST_SNAKE_SRC = test/test_snake/test_snake_game.cpp \
				test/test_snake/test_main.cpp \
                 test/test_snake/test_snake_fsm.cpp

TEST_TETRIS_SRC = test/test_tetris/test_tetris_game.cpp \
                  test/test_tetris/test_tetris_fsm.cpp \
                  test/test_tetris/test_main.cpp

TEST_SNAKE_BIN = test/test_snake_bin
TEST_TETRIS_BIN = test/test_tetris_bin

# Тесты Snake (ваши существующие)
test_snake: $(LIBSNAKE) $(TEST_SNAKE_SRC)
	@echo "=== Building Snake tests ==="
	$(CXX) $(CXXFLAGS) -o $(TEST_SNAKE_BIN) $(TEST_SNAKE_SRC) -L. -lsnake -lgtest -lgtest_main -lpthread
	@echo "=== Running Snake tests ==="
	LD_LIBRARY_PATH=. ./$(TEST_SNAKE_BIN)

# Тесты Tetris (новые)
test_tetris: $(LIBTETRIS) $(TEST_TETRIS_SRC)
	@echo "=== Building Tetris tests ==="
	$(CXX) $(CXXFLAGS) -o $(TEST_TETRIS_BIN) $(TEST_TETRIS_SRC) -L. -ltetris -lgtest -lgtest_main -lpthread
	@echo "=== Running Tetris tests ==="
	LD_LIBRARY_PATH=. ./$(TEST_TETRIS_BIN)

# Все тесты
test: test_snake test_tetris
	@echo "=== All tests completed ==="

# Покрытие кода (библиотек)
coverage: CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage: clean_libs $(LIBSNAKE) $(LIBTETRIS) test
	@echo "=== Generating coverage report for libraries ==="
	gcov -r src/brickgame/snake/*.cpp src/brickgame/tetris/*.c
	lcov --capture --directory . --output-file test/coverage.info
	lcov --remove test/coverage.info '/usr/*' '/opt/*' '/tmp/*' 'test_*' --output-file test/coverage_filtered.info
	genhtml test/coverage_filtered.info --output-directory test/coverage_report --title "BrickGame Libraries Coverage"

# LCOV отчет с HTML (покрытие библиотек)
lcov: CXXFLAGS += -fprofile-arcs -ftest-coverage
lcov: clean_libs $(LIBSNAKE) $(LIBTETRIS) test
	@echo "=== Generating LCOV HTML report for libraries ==="
	lcov --capture --directory . --output-file test/coverage.info
	lcov --remove test/coverage.info '/usr/*' '/opt/*' '/tmp/*' 'test_*' --output-file test/coverage_filtered.info
	genhtml test/coverage_filtered.info --output-directory test/lcov_report --title "BrickGame Libraries Coverage Report"
	@echo "=== LCOV report generated in test/lcov_report/index.html ==="

# Очистка библиотек для пересборки с покрытием
clean_libs:
	@echo "=== Cleaning libraries for coverage build ==="
	rm -f $(LIBSNAKE) $(LIBTETRIS)
	rm -f src/brickgame/snake/*.gcno src/brickgame/snake/*.gcda
	rm -f src/brickgame/tetris/*.gcno src/brickgame/tetris/*.gcda
	rm -f test/*.gcno test/*.gcda

.PHONY: all clean install uninstall dist snake_qt test_snake test_tetris test coverage lcov clean_libs

