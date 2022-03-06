#include <SDL2/SDL.h>
#include "Maps.h"



const int BORDER_THICKNESS = 10;

Maps makeMap(int height, int width, int** mapArray) {
    Maps m = {
      .height = height,
      .width = width,
      .map_array = mapArray,
      .startingPosX = width / 2,
      .startingPosY = height / 2,
    };
    return m;
}

void renderMap(SDL_Renderer **renderer, Maps maps, int screenHeight, int screenWidth) {
    int xOffSetToCenter = (screenWidth / 2) - (maps.width / 2);
    int yOffSetToCenter = (screenHeight / 2) - (maps.height / 2);

    SDL_SetRenderDrawColor(*renderer, 180, 180, 180, 1);
    SDL_Rect playable_area = {
            .x = xOffSetToCenter,
            .y = yOffSetToCenter,
            .w = maps.width,
            .h = maps.height,
    };
    SDL_RenderFillRect(*renderer, &playable_area);


    for (int y = 0; y < maps.height; ++y) {
        for (int x = 0; x < maps.height; ++x) {
            if (maps.map_array[y][x] == 1) {
                SDL_SetRenderDrawColor(*renderer, 180, 180, 180, 1);
                SDL_Rect border_block = {
                        .x = xOffSetToCenter + x,
                        .y = yOffSetToCenter + y,
                        .w = BORDER_THICKNESS,
                        .h = BORDER_THICKNESS
                };
                SDL_RenderFillRect(*renderer, &border_block);
            }
        }
    }




}







