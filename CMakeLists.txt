cmake_minimum_required(VERSION 3.21)
project(COMP4985_Game_Prototype_v0 C)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)

add_executable(COMP4985_Game_Prototype_v0 main.c include/State/state.h include/Players/Players.c include/Players/Players.h include/Maps/Maps.c include/Maps/Maps.h include/Players/Bullets.c include/Players/Bullets.h include/Monsters/Monsters.c include/Monsters/Monsters.h include/Monsters_v2/Monsters_v2.c include/Monsters_v2/Monsters_v2.h include/Boosts/Boosts.c include/Boosts/Boosts.h)
target_link_libraries(COMP4985_Game_Prototype_v0 ${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(COMP4985_Game_Prototype_v0 -lm ${LIBS})