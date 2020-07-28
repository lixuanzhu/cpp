cmake_minimum_required (VERSION 3.11)

project (tutorial)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git)
FetchContent_MakeAvailable(cpr)

add_executable (tutorial tutorial.cpp)
target_link_libraries(tutorial PRIVATE cpr::cpr)
