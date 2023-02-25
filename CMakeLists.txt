cmake_minimum_required(VERSION 3.16)

project(podiceps2 LANGUAGES CXX)

#Enable debugging flags
option(DEBUGGING "Debugging options" ON)
if(DEBUGGING)
    add_compile_options(-std=c++11 -Wall -g)
else()
    add_compile_options(-std=c++11 -O2)
endif(DEBUGGING)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

add_executable(podiceps2
    dictionary_widget.cpp
    main.cpp
    main_window.cpp
    save_dialog.cpp
    table_model.cpp
    word_line.cpp
)

qt5_use_modules(podiceps2 Widgets)
