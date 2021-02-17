cmake_minimum_required(VERSION 3.4)
project(trn-server VERSION 0.1.0 LANGUAGES C)

option(BUILD_TESTS "Build test programs" ON)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wconversion -Werror -Wextra -Wshadow -Wundef -Wunreachable-code -pedantic-errors")

find_path(LIBDUC_INCLUDE_DIR duc.h)
find_library(LIBDUC_LIBRARY duc)

find_path(LIBUV_INCLUDE_DIR uv.h)
find_library(LIBUV_LIBRARY uv)

include_directories("${LIBDUC_INCLUDE_DIR}" "${LIBUV_INCLUDE_DIR}")
link_libraries("${LIBDUC_LIBRARY}" "${LIBUV_LIBRARY}")

set(sources src/game-server.c)
add_executable(${PROJECT_NAME} ${sources} src/main.c)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
  set(CTEST_MEMORYCHECK_COMMAND valgrind)
  set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full --show-leak-kinds=all '--suppressions=${PROJECT_SOURCE_DIR}/valgrind.supp' --tool=memcheck --track-origins=yes")

  include(CTest)
  set(tests game-server)

  foreach (test ${tests})
    add_executable(${PROJECT_NAME}-test-${test} ${sources} tests/${test}-test.c)
    add_test(${test} ${PROJECT_NAME}-test-${test})
  endforeach ()
endif ()
