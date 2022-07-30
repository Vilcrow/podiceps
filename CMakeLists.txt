cmake_minimum_required(VERSION 3.16)

project(podiceps2 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

add_executable(podiceps2
    dictionarywidget.cpp
    main.cpp
    mainwindow.cpp 
    savedialog.cpp
    tablemodel.cpp
)

qt5_use_modules(podiceps2 Widgets)
