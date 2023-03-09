set(CMAKE_AUTOMOC ON)

set(APP_LIB_SOURCE
    dictionary_widget.cpp
    input_widget.cpp
    main_window.cpp
    podiceps_application.cpp
    preferences_widget.cpp
    save_dialog.cpp
    table_model.cpp
    table_widget.cpp
    word_card.cpp
    word_line.cpp
)

add_library(${APP_LIB_NAME} ${APP_LIB_SOURCE})

add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME} ${APP_LIB_NAME})

find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)
target_link_libraries(${APP_LIB_NAME} Qt5::Widgets Qt5::Xml)
