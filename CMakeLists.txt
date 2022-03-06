cmake_minimum_required(VERSION 3.21)
project(COMP4985_Game_Prototype_v0 C)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(COMP4985_Game_Prototype_v0 main.c include/State/state.h include/Players/Players.c include/Players/Players.h)
target_link_libraries(COMP4985_Game_Prototype_v0 ${SDL2_LIBRARIES})