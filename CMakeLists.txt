cmake_minimum_required(VERSION 3.16)

project(podiceps LANGUAGES CXX)

#Enable debugging flags
option(DEBUGGING "Debugging options" ON)
if(DEBUGGING)
    add_compile_options(-std=c++11 -Wall -g)
else()
    add_compile_options(-std=c++11 -O2)
endif(DEBUGGING)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)

add_executable(podiceps
    dictionary_widget.cpp
    input_widget.cpp
    main.cpp
    main_window.cpp
    save_dialog.cpp
    table_model.cpp
    table_widget.cpp
    word_line.cpp
)

target_link_libraries(podiceps Qt5::Widgets Qt5::Xml)
