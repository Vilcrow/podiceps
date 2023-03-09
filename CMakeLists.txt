cmake_minimum_required(VERSION 3.16)

project(podiceps LANGUAGES CXX)

#Enable debugging flags
option(DEBUGGING "Debugging options" ON)
if(DEBUGGING)
    add_compile_options(-std=c++11 -Wall -g)
else()
    add_compile_options(-std=c++11 -O2)
endif(DEBUGGING)

set(APP_NAME podiceps)
set(APP_LIB_NAME podiceps_lib)

#Directory with source code
add_subdirectory(src)

#Compilaton of the tests
option(COMPILE_UTESTS "Compile the unit tests" ON)
if(COMPILE_UTESTS)
    add_subdirectory(tests)
endif(COMPILE_UTESTS)
