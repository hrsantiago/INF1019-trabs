CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(sim-virtual)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -fno-inline -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS_RELEASE "-O4")
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c99")

ADD_EXECUTABLE(sim-virtual
src/debug.h
src/log.c
src/log.h
src/main.c
src/pagetable.c
src/pagetable.h
)

TARGET_LINK_LIBRARIES(sim-virtual m)
