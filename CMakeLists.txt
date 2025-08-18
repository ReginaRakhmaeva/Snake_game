cmake_minimum_required(VERSION 3.16)
project(BrickGameSnakeQt VERSION 1.0 LANGUAGES CXX)

# Указываем стандарт
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# Ищем Qt6 (или Qt5 при необходимости)
find_package(Qt6 6.2 COMPONENTS Widgets REQUIRED)

# Исходники Qt-версии
set(SOURCES
    src/gui/desktop/main.cpp
    src/gui/desktop/mainwindow.cpp
    src/gui/desktop/gamewidget.cpp
    src/brickgame/snake/snake_api.cpp
    src/brickgame/snake/snake_fsm.cpp
    src/brickgame/snake/snake_game.cpp
)

set(HEADERS
    include/gui/desktop/mainwindow.h
    include/gui/desktop/gamewidget.h
)

# Для автоматической генерации moc-файлов
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Создаем исполняемый файл
add_executable(snake_qt ${SOURCES} ${HEADERS})

# Линкуем с Qt
target_link_libraries(snake_qt PRIVATE Qt6::Widgets)

# Опционально — включить строгие предупреждения
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(snake_qt PRIVATE -Wall -Wextra -pedantic)
elseif (MSVC)
    target_compile_options(snake_qt PRIVATE /W4 /permissive-)
endif()
