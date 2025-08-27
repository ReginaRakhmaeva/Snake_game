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
	rm -f cppcheck-report.xml
	rm -f valgrind-out.txt
	
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
	
	# Удаляем документацию и архивы
	rm -rf doc/
	rm -rf dist/
	rm -f Doxyfile
	
	@echo "=== Clean completed ==="


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

# Все тесты с отчетами покрытия
test_with_coverage: coverage lcov
	@echo "=== All tests and coverage reports completed ==="
	@echo "=== Coverage reports available in: ==="
	@echo "  - test/coverage_report/index.html"
	@echo "  - test/lcov_report/index.html"

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
		doxygen Doxyfile 2>/dev/null || echo "Doxyfile not found, creating default..."; \
		echo "Creating default Doxyfile..."; \
		echo "PROJECT_NAME = BrickGame" > Doxyfile; \
		echo "PROJECT_NUMBER = 1.0" >> Doxyfile; \
		echo "OUTPUT_DIRECTORY = doc" >> Doxyfile; \
		echo "INPUT = src include" >> Doxyfile; \
		echo "RECURSIVE = YES" >> Doxyfile; \
		echo "GENERATE_HTML = YES" >> Doxyfile; \
		echo "GENERATE_LATEX = YES" >> Doxyfile; \
		doxygen Doxyfile; \
		@echo "Documentation generated in doc/"; \
	else \
		echo "Doxygen not found. Please install doxygen to generate documentation."; \
	fi

# === Создание архива ===
dist: clean
	@echo "=== Creating distribution archive ==="
	@mkdir -p dist
	@VERSION=$$(date +%Y%m%d); \
	tar -czf dist/brickgame-$$VERSION.tar.gz \
		--exclude='.git' \
		--exclude='build_qt' \
		--exclude='*.gcda' \
		--exclude='*.gcno' \
		--exclude='test/*.gcda' \
		--exclude='test/*.gcno' \
		--exclude='test/test_snake_bin' \
		--exclude='test/test_tetris_bin' \
		--exclude='test/coverage_report' \
		--exclude='test/lcov_report' \
		--exclude='test/coverage.info' \
		--exclude='test/coverage_filtered.info' \
		--exclude='*.log' \
		--exclude='*.dSYM' \
		--exclude='*.o' \
		--exclude='*.so' \
		--exclude='*.dylib' \
		--exclude='*.dll' \
		--exclude='*.exe' \
		--exclude='*.app' \
		--exclude='brickgame_cli' \
		--exclude='brickgame_desktop' \
		--exclude='libtetris.so' \
		--exclude='libsnake.so' \
		--exclude='libtetris.dylib' \
		--exclude='libsnake.dylib' \
		--exclude='libtetris.dll' \
		--exclude='libsnake.dll' \
		--exclude='snake_highscore.txt' \
		--exclude='tetris_highscore.txt' \
		--exclude='dist' \
		--exclude='doc' \
		--exclude='CMakeCache.txt' \
		--exclude='CMakeFiles' \
		--exclude='cmake_install.cmake' \
		--exclude='Makefile.cmake' \
		--exclude='*.moc' \
		--exclude='moc_*.cpp' \
		--exclude='ui_*.h' \
		--exclude='qrc_*.cpp' \
		--exclude='*_autogen' \
		--exclude='.DS_Store' \
		--exclude='Thumbs.db' \
		--exclude='*~' \
		--exclude='.#*' \
		--exclude='#*#' \
		--exclude='.vscode' \
		--exclude='.idea' \
		--exclude='*.swp' \
		--exclude='*.swo' \
		--exclude='brickgame-*.tar.gz' \
		--exclude='brickgame-*.zip' \
		. ; \
	echo "Distribution archive created: dist/brickgame-$$VERSION.tar.gz"

# === Проверка утечек памяти ===
valgrind_snake: clean $(LIBSNAKE) $(TEST_SNAKE_SRC)
	@echo "=== Checking Snake library for memory leaks ==="
	$(CXX) $(CXXFLAGS) -o $(TEST_SNAKE_BIN) $(TEST_SNAKE_SRC) -L. -lsnake -lgtest -lgtest_main -lpthread
	@if command -v valgrind >/dev/null 2>&1; then \
		LD_LIBRARY_PATH=. valgrind -s --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=1 \
		--suppressions=valgrind.supp ./$(TEST_SNAKE_BIN); \
	else \
		echo "Valgrind not found. Please install valgrind to check for memory leaks."; \
		LD_LIBRARY_PATH=. ./$(TEST_SNAKE_BIN); \
	fi

valgrind_tetris: clean $(LIBTETRIS) $(TEST_TETRIS_SRC)
	@echo "=== Checking Tetris library for memory leaks ==="
	$(CXX) $(CXXFLAGS) -o $(TEST_TETRIS_BIN) $(TEST_TETRIS_SRC) -L. -ltetris -lgtest -lgtest_main -lpthread
	@if command -v valgrind >/dev/null 2>&1; then \
		LD_LIBRARY_PATH=. valgrind -s --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=1 \
		--suppressions=valgrind.supp ./$(TEST_TETRIS_BIN); \
	else \
		echo "Valgrind not found. Please install valgrind to check for memory leaks."; \
		LD_LIBRARY_PATH=. ./$(TEST_TETRIS_BIN); \
	fi

# Проверка утечек для всех компонентов
valgrind: valgrind_snake valgrind_tetris
	@echo "=== All memory leak checks completed ==="

# === Статический анализ кода ===
# Проверка с помощью cppcheck
cppcheck:
	@echo "=== Running static analysis with cppcheck ==="
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "Checking C files..."; \
		cppcheck --enable=all --std=c11 --language=c --suppress=missingIncludeSystem \
		src/brickgame/tetris/*.c src/gui/cli/*.c; \
		echo "Checking C++ files..."; \
		cppcheck --enable=all --std=c++20 --language=c++ --suppress=missingIncludeSystem \
		src/brickgame/snake/*.cpp include/; \
	else \
		echo "Cppcheck not found. Please install cppcheck to run static analysis."; \
	fi

.PHONY: all clean install uninstall dvi dist snake_qt test_snake test_tetris test coverage lcov clean_libs valgrind_snake valgrind_tetris valgrind cppcheck 

