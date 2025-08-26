cmake_minimum_required(VERSION 3.16)
project(brickgame_desktop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение операционной системы
if(APPLE)
    set(SHARED_EXT ".dylib")
elseif(WIN32)
    set(SHARED_EXT ".dll")
else()
    set(SHARED_EXT ".so")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(DESKTOP_SOURCES
    src/gui/desktop/main.cpp
    src/gui/desktop/mainwindow.cpp
    src/gui/desktop/gamewidget.cpp
    src/gui/desktop/gamecontroller.cpp
    src/gui/desktop/libraryloader.cpp
    src/gui/desktop/gameselectiondialog.cpp
    src/gui/desktop/gameoverdialog.cpp
)

set(DESKTOP_HEADERS
    include/gui/desktop/mainwindow.h
    include/gui/desktop/gamewidget.h
    include/gui/desktop/gamecontroller.h
    include/gui/desktop/libraryloader.h
    include/gui/desktop/gameselectiondialog.h
    include/gui/desktop/gameoverdialog.h
)

add_executable(brickgame_desktop ${DESKTOP_SOURCES} ${DESKTOP_HEADERS})
target_link_libraries(brickgame_desktop Qt6::Core Qt6::Widgets)
target_include_directories(brickgame_desktop PRIVATE include)

# Кроссплатформенное подключение библиотек
if(APPLE)
    target_link_libraries(brickgame_desktop ${CMAKE_CURRENT_SOURCE_DIR}/libtetris${SHARED_EXT} ${CMAKE_CURRENT_SOURCE_DIR}/libsnake${SHARED_EXT} dl)
elseif(WIN32)
    target_link_libraries(brickgame_desktop ${CMAKE_CURRENT_SOURCE_DIR}/libtetris${SHARED_EXT} ${CMAKE_CURRENT_SOURCE_DIR}/libsnake${SHARED_EXT})
else()
    target_link_libraries(brickgame_desktop ${CMAKE_CURRENT_SOURCE_DIR}/libtetris${SHARED_EXT} ${CMAKE_CURRENT_SOURCE_DIR}/libsnake${SHARED_EXT} dl)
endif()

set_target_properties(brickgame_desktop PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
