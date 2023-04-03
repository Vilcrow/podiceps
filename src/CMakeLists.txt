set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(APP_LIB_SOURCE
    about_widget.cpp
    action_add.cpp
    action_base.cpp
    action_delete.cpp
    action_edit.cpp
    action_log.cpp
    dictionary_widget.cpp
    file_info_widget.cpp
    find_widget.cpp
    header_menu.cpp
    input_widget.cpp
    main_window.cpp
    manual_widget.cpp
    podiceps_application.cpp
    preferences_widget.cpp
    save_dialog.cpp
    settings.cpp
    statistics_widget.cpp
    status_spin_box.cpp
    table_model.cpp
    table_widget.cpp
    text_file.cpp
    word_add.cpp
    word_edit.cpp
    word_line.cpp
    word_status.cpp
)

add_library(${APP_LIB_NAME} ${APP_LIB_SOURCE})

add_executable(${APP_NAME} main.cpp ../resources/resources.qrc)
target_link_libraries(${APP_NAME} ${APP_LIB_NAME})

find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)
target_link_libraries(${APP_LIB_NAME} Qt5::Widgets Qt5::Xml)
