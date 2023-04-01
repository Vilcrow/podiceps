set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Test REQUIRED)

set(TEST_APP_NAME ${APP_NAME}_tests)

set(TEST_SOURCES
    main_test.cpp
    status_spin_box_test.cpp
    word_add_test.cpp
    word_line_test.cpp
    word_status_test.cpp
)

include_directories(SRC_INCLUDE_DIR ../src)

add_executable(${TEST_APP_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_APP_NAME} ${APP_LIB_NAME} Qt5::Test)

add_custom_command(TARGET ${TEST_APP_NAME}
                   COMMAND ./${TEST_APP_NAME} -silent
                   POST_BUILD)
