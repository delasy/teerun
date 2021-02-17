cmake_minimum_required(VERSION 3.14)
project(trn-client VERSION 0.1.0 LANGUAGES C)

include(FetchContent)

set(GLAD_INSTALL ON)

FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.34)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.3)

FetchContent_MakeAvailable(glad glfw)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wconversion -Werror -Wextra -Wshadow -Wundef -Wunreachable-code -pedantic-errors")

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
