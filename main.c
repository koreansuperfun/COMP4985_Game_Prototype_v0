#include <stdio.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "include/State/state.h"
#include "include/Players/Players.h"


bool initialize(void);
void update(float elapsed);
void shutdown();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Player player1;
int numOfPlayers = 1;


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
    current_state = MAIN_GAME_STATE;
    quit = false;

    while (!quit) {
        //One and important way to kill the game.
        //TODO: Need to set the quit to true when our FMS is EXIT_STATE.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        uint32_t curTick = SDL_GetTicks();
        uint32_t diff = curTick - lastTick;
        float elapsed = (float) diff / 1000.0f;
        update(elapsed);
        lastTick = curTick;


    }
    SDL_Quit();


    return 0;
}

bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Pong",
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

    player1 = makeNewPlayer();


    return true;
}

void update(float elapsed) {
    SDL_SetRenderDrawColor(renderer, 128, 192, 255, 255);
    SDL_RenderClear(renderer);

    updatePlayers(&player1, 1, elapsed);
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

    SDL_Quit();
}
