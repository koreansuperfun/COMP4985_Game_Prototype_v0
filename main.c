#include <stdio.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "include/State/state.h"
#include "include/Players/Players.h"
#include "include/Maps/Maps.h"


bool initialize(void);
void update(float elapsed);
void shutdown();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
const int SCREEN_WIDTH = 1620;
const int SCREEN_HEIGHT = 880;

Player player1;
int numOfPlayers = 1;

//Map Section
Maps maps;
int **map_array;
const int MAP_DEFAULT_HEIGHT = 750;
const int MAP_DEFAULT_WIDTH = 1600;

const int BORDER_VALUE = 50;



int main() {
    SDL_Event event;
    uint32_t lastTick;
    atexit(shutdown);
    enum game_state current_state;
    bool quit;


    if(!initialize()) {
        exit(1);
    }

    lastTick = SDL_GetTicks();
    current_state = INIT_STATE;
    quit = false;

    while (!quit) {
        switch(current_state) {
            case INIT_STATE:
                SDL_PollEvent(&event);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

                if (keyboardState[SDL_SCANCODE_SPACE]) {
                    current_state = MAIN_GAME_STATE;
                }
//                while(SDL_PollEvent(&event)) {
//                    if (event.type == SDL_QUIT) {
//                        quit = true;
//                    }
//                    if (event.type == SDL_KEYDOWN) {
//                        current_state = MAIN_GAME_STATE;
//                    }
//                }
                break;
            case MAIN_GAME_STATE:
//                printf("Arrived at Game State\n");
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }
                    switch(event.type){
                        case SDL_QUIT:
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            printf("Left Mouse Button Successfully Pressed at x:%d, y:%d\n",
                                   event.button.x, event.button.y);
                            addNewBullet(player1, event.button.x, event.button.y);
                            break;

                    }
                }

                uint32_t curTick = SDL_GetTicks();
                uint32_t diff = curTick - lastTick;
                float elapsed = (float) diff / 1000.0f;
                update(elapsed);
                lastTick = curTick;
                break;

            case EXIT_STATE:
                quit = true;
            case SCOREBOARD_STATE:
                break;
            default:
                break;
        }
    }

    SDL_Quit();


    return 0;
}

bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("COMP4985 Prototype v0",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }

    //index: index of graphic (?). -1 is default
    //SDL_RENDER_ACCELERATED: use our graphic card to do the rendering.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window) {
        return false;
    }



    map_array = (int**)malloc(MAP_DEFAULT_HEIGHT * sizeof(int*));
    for (int i = 0; i < MAP_DEFAULT_HEIGHT; ++i) {
        map_array[i] = (int*) malloc(MAP_DEFAULT_WIDTH * sizeof(int));
    }
    for (int i = 0; i < MAP_DEFAULT_HEIGHT; ++i) {
        for (int j = 0; j < MAP_DEFAULT_WIDTH; ++j) {
            map_array[i][j] = 0;
        }
    }




    maps = makeMap(MAP_DEFAULT_HEIGHT, MAP_DEFAULT_WIDTH, map_array);
    setBorderLine(&maps,100, 100, MAP_DEFAULT_WIDTH - 200, 'r');
    setBorderLine(&maps,100, 100, 550, 'd');
    setBorderLine(&maps,1500, 650, 550, 'u');
    setBorderLine(&maps,1500, 650, MAP_DEFAULT_WIDTH - 200, 'l');


    player1 = makeNewPlayer(maps.width / 2, maps.height / 2);



    return true;
}

void update(float elapsed) {


    SDL_SetRenderDrawColor(renderer, 128, 192, 255, 255);
    SDL_RenderClear(renderer);

    renderMap(&renderer, maps, SCREEN_HEIGHT, SCREEN_WIDTH);
    int xOffSetToCenter = (SCREEN_WIDTH / 2) - (maps.width / 2);
    int yOffSetToCenter = (SCREEN_HEIGHT / 2) - (maps.height / 2);
    updatePlayers(&player1, numOfPlayers, elapsed, map_array,
                  MAP_DEFAULT_WIDTH, MAP_DEFAULT_HEIGHT,
                  xOffSetToCenter, yOffSetToCenter);
    renderPlayers(&renderer, &player1, 1);

    SDL_RenderPresent(renderer);
}

void shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    if (map_array) {
        for (int i = 0; i < MAP_DEFAULT_HEIGHT; ++i) {
            free(map_array[i]);
        }
        free(map_array);
    }


    SDL_Quit();
}
