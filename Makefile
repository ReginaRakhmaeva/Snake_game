# === Конфигурация ===
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11 -g \
           -Iinclude/brickgame/common \
           -Iinclude/brickgame/tetris \
           -Iinclude/brickgame/frontend/cli
LDFLAGS := -lncurses

# Папки
TETRIS_SRC  := src/brickgame/tetris
CLI_SRC     := src/frontend/cli
BUILD_DIR   := build

# Исходники
TETRIS_SRCS := $(wildcard $(TETRIS_SRC)/*.c)
CLI_SRCS    := $(wildcard $(CLI_SRC)/*.c)

# Объекты
TETRIS_OBJS := $(patsubst $(TETRIS_SRC)/%.c,$(BUILD_DIR)/tetris_%.o,$(TETRIS_SRCS))
CLI_OBJS    := $(patsubst $(CLI_SRC)/%.c,$(BUILD_DIR)/cli_%.o,$(CLI_SRCS))

# Цели
TARGET      := tetris

# === Правила ===

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(TETRIS_OBJS) $(CLI_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/tetris_%.o: $(TETRIS_SRC)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cli_%.o: $(CLI_SRC)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
