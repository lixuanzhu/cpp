cmake_minimum_required (VERSION 3.11)

project (hello)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")
find_package(cpprestsdk REQUIRED)

ADD_LIBRARY(boost STATIC IMPORTED)
SET_TARGET_PROPERTIES(boost PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_system.a)

add_executable (${PROJECT_NAME} h3.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE cpprestsdk::cpprest boost)
