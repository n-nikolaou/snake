cmake_minimum_required(VERSION 3.21)
project(game)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR C:/Users/nickn/Documents/SDL2-2.24.0/include)
set(SDL2_LIB_DIR C:/Users/nickn/Documents/SDL2-2.24.0/lib/x86)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(game main.cpp snake.cpp snake.h food.cpp food.h score.h score.cpp display_text.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf)