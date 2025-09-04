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
TETRIS_SRC = brickgame/tetris/backend.c \
             brickgame/tetris/fsm.c \
             brickgame/tetris/game.c

SNAKE_SRC  = brickgame/snake/snake_api.cpp \
             brickgame/snake/snake_fsm.cpp \
             brickgame/snake/snake_game.cpp

CLI_SRC    = gui/cli/main.c \
             gui/cli/input.c \
             gui/cli/render.c \
             gui/cli/app_controller.c

# === Библиотеки ===
LIBTETRIS = libtetris$(SHARED_EXT)
LIBSNAKE  = libsnake$(SHARED_EXT)

# === Исходники для Qt проекта ===
DESKTOP_SRC = gui/desktop/main.cpp \
              gui/desktop/mainwindow.cpp \
              gui/desktop/gamewidget.cpp \
              gui/desktop/gamecontroller.cpp \
              gui/desktop/libraryloader.cpp \
              gui/desktop/gameselectiondialog.cpp \
              gui/desktop/gameoverdialog.cpp \
              gui/desktop/inputhandler.cpp \
              gui/desktop/timermanager.cpp

# === Qt проект ===
QT_BUILD_DIR = build_qt

# === Цели ===
all: $(LIBTETRIS) $(LIBSNAKE) brickgame_cli brickgame_desktop

open_cli: 
	./brickgame_cli
open_desktop: 
	./brickgame_desktop

$(LIBTETRIS): $(TETRIS_SRC)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -o $@ $(TETRIS_SRC)

$(LIBSNAKE): $(SNAKE_SRC)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $(SNAKE_SRC)

brickgame_cli: $(LIBTETRIS) $(LIBSNAKE) $(CLI_SRC)
	$(CC) $(CFLAGS) -o $@ $(CLI_SRC) $(LDFLAGS)

brickgame_desktop: $(LIBTETRIS) $(LIBSNAKE)
	@echo "=== Building Qt frontend ==="
	@mkdir -p build_qt
	cd build_qt && cmake "$$(pwd)/.." && make



# === Каталог установки ===
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include/brickgame

# Проверка прав на запись в системные директории
check_permissions:
	@if [ ! -w $(PREFIX) ]; then \
		echo "  1. Run 'sudo make install' "; \
		exit 1; \
	fi

install: all check_permissions
	@echo "=== Installing BrickGame ==="
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(INCLUDEDIR)
	cp brickgame_cli $(BINDIR)/
	cp $(LIBTETRIS) $(LIBDIR)/
	cp $(LIBSNAKE) $(LIBDIR)/
	cp -r include/brickgame/* $(INCLUDEDIR)/
	@echo "Installed to $(BINDIR), $(LIBDIR), $(INCLUDEDIR)"
	@echo "You may need to run 'sudo ldconfig' to update library cache"

uninstall: check_permissions
	@echo "=== Uninstalling BrickGame ==="
	rm -f $(BINDIR)/brickgame_cli
	rm -f $(LIBDIR)/$(LIBTETRIS)
	rm -f $(LIBDIR)/$(LIBSNAKE)
	rm -rf $(INCLUDEDIR)
	@echo "Uninstalled from $(BINDIR), $(LIBDIR), $(INCLUDEDIR)"

clean:
	@echo "=== Cleaning build artifacts ==="
	# Удаляем исполняемые файлы и библиотеки
	rm -f libtetris.so libsnake.so brickgame_cli brickgame_desktop
	
	rm -rf *.dSYM
	rm -rf libtetris.dylib.dSYM libsnake.dylib.dSYM brickgame_cli.dSYM brickgame_desktop.dSYM
	rm -rf *.dylib
	# Удаляем объектные файлы
	rm -f *.o
	
	# Удаляем файлы покрытия кода
	rm -f *.gcda *.gcno *.gcov
	
	# Удаляем высокие счета
	rm -f snake_highscore.txt tetris_highscore.txt
	
	# Удаляем тестовые исполняемые файлы
	rm -f test/test_snake_bin test/test_tetris_bin
	
	# Удаляем файлы покрытия тестов
	rm -f test/*.gcno test/*.gcda
	rm -f test/coverage.info test/coverage_filtered.info
	rm -rf test/coverage_report test/lcov_report
	
	# Удаляем директории сборки Qt
	rm -rf build_qt
	
	# Удаляем документацию и архивы
	rm -rf doc/
	rm -rf dist/
	rm -f Doxyfile
	
	# Удаляем macOS служебные файлы
	find . -name "._*" -type f -delete
	find . -name ".DS_Store" -type f -delete


	@echo "=== Clean completed ==="


# === Форматирование кода ===
format_check:
	cp ../materials/linters/.clang-format .clang-format
	find . -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp"  | xargs clang-format -n
	rm -f .clang-format

format_fix:
	cp ../materials/linters/.clang-format .clang-format
	find . -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" | xargs clang-format -i
	rm -f .clang-format


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
coverage:
	@echo "=== Building and running tests with coverage ==="
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -shared -fPIC $(SNAKE_SRC) -o $(LIBSNAKE)
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -shared -fPIC $(TETRIS_SRC) -o $(LIBTETRIS)
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -o $(TEST_SNAKE_BIN) $(TEST_SNAKE_SRC) -L. -lsnake -lgtest -lgtest_main -lpthread
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -o $(TEST_TETRIS_BIN) $(TEST_TETRIS_SRC) -L. -ltetris -lgtest -lgtest_main -lpthread
	LD_LIBRARY_PATH=. ./$(TEST_SNAKE_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_TETRIS_BIN)
	@echo "=== Generating coverage report ==="
	gcov -r src/brickgame/snake/*.cpp src/brickgame/tetris/*.c
	lcov --capture --directory . --output-file test/coverage.info
	lcov --remove test/coverage.info '/usr/*' '/opt/*' '/tmp/*' 'test_*' --output-file test/coverage_filtered.info
	genhtml test/coverage_filtered.info --output-directory test/coverage_report --title "BrickGame Libraries Coverage"

# LCOV отчет с HTML (покрытие библиотек)
lcov:
	@echo "=== Building and running tests with coverage ==="
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -shared -fPIC $(SNAKE_SRC) -o $(LIBSNAKE)
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -shared -fPIC $(TETRIS_SRC) -o $(LIBTETRIS)
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -o $(TEST_SNAKE_BIN) $(TEST_SNAKE_SRC) -L. -lsnake -lgtest -lgtest_main -lpthread
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage -o $(TEST_TETRIS_BIN) $(TEST_TETRIS_SRC) -L. -ltetris -lgtest -lgtest_main -lpthread
	LD_LIBRARY_PATH=. ./$(TEST_SNAKE_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_TETRIS_BIN)
	@echo "=== Generating LCOV HTML report ==="
	lcov --capture --directory . --output-file test/coverage.info
	lcov --remove test/coverage.info '/usr/*' '/opt/*' '/tmp/*' 'test_*' --output-file test/coverage_filtered.info
	genhtml test/coverage_filtered.info --output-directory test/lcov_report --title "BrickGame Libraries Coverage Report"
	@echo "=== LCOV report generated in test/lcov_report/index.html ==="



# === Создание документации ===
dvi:
	@echo "=== Generating documentation ==="
	@if command -v doxygen >/dev/null 2>&1; then \
		rm -rf doc; \
		echo "Creating default Doxyfile..."; \
		echo "PROJECT_NAME = BrickGame" > Doxyfile; \
		echo "PROJECT_NUMBER = 1.0" >> Doxyfile; \
		echo "OUTPUT_DIRECTORY = doc" >> Doxyfile; \
		echo "INPUT = src include" >> Doxyfile; \
		echo "RECURSIVE = YES" >> Doxyfile; \
		echo "GENERATE_HTML = YES" >> Doxyfile; \
		echo "GENERATE_LATEX = NO" >> Doxyfile; \
		echo "HAVE_DOT = NO" >> Doxyfile; \
		echo "CLASS_DIAGRAMS = NO" >> Doxyfile; \
		echo "COLLABORATION_GRAPH = NO" >> Doxyfile; \
		echo "UML_LOOK = NO" >> Doxyfile; \
		echo "TEMPLATE_RELATIONS = NO" >> Doxyfile; \
		echo "INCLUDE_GRAPH = NO" >> Doxyfile; \
		echo "INCLUDED_BY_GRAPH = NO" >> Doxyfile; \
		echo "CALL_GRAPH = NO" >> Doxyfile; \
		echo "CALLER_GRAPH = NO" >> Doxyfile; \
		echo "GRAPHICAL_HIERARCHY = NO" >> Doxyfile; \
		echo "DIRECTORY_GRAPH = NO" >> Doxyfile; \
		doxygen Doxyfile 2>/dev/null || true; \
		echo "Documentation generated in doc/"; \
	else \
		echo "Doxygen not found. Please install doxygen to generate documentation."; \
	fi

# === Создание архива ===
dist: clean
	@echo "=== Creating distribution archive ==="
	@mkdir -p dist
	@VERSION=$$(date +%Y%m%d); \
	tar -czf dist/brickgame-$$VERSION.tar.gz \
		--exclude='.gitkeep' \
		--exclude='dist' \
		--exclude='.vscode' \
		. ; \
	echo "Distribution archive created: dist/brickgame-$$VERSION.tar.gz"

# === Проверка утечек памяти ===
valgrind_cli: 
	@echo "=== Checking Snake library for memory leaks ==="
	valgrind --tool=memcheck --leak-check=yes  ./brickgame_cli 


valgrind_desktop: 
	@echo "=== Checking Tetris library for memory leaks ==="
	valgrind --tool=memcheck --leak-check=yes  ./brickgame_desktop 

# Проверка утечек для всех компонентов
valgrind: valgrind_cli valgrind_desktop
	@echo "=== All memory leak checks completed ==="


.PHONY: all clean install uninstall dvi dist snake_qt test_snake test_tetris test coverage lcov clean_libs valgrind format_check format_fix

