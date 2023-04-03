cmake_minimum_required(VERSION 3.16)

project(podiceps LANGUAGES CXX VERSION 0.1)

#Enable debugging flags
option(DEBUGGING "Debugging options" ON)
if(DEBUGGING)
    add_compile_options(-std=c++11 -Wall -g)
    add_compile_definitions(QT_MESSAGELOGCONTEXT)
else()
    add_compile_options(-std=c++11 -O2)
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif(DEBUGGING)

set(APP_NAME podiceps)
add_compile_definitions(PODICEPS_VERSION="${CMAKE_PROJECT_VERSION}")

set(APP_LIB_NAME podiceps_lib)

#Directory with resources
#add_subdirectory(resources)

#Directory with source code
add_subdirectory(src)

#Compilaton of the tests
option(COMPILE_UTESTS "Compile the unit tests" ON)
if(COMPILE_UTESTS)
    add_subdirectory(tests)
endif(COMPILE_UTESTS)
