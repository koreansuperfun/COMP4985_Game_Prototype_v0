#include <SDL2/SDL.h>
#include "Players.h"

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;
const int PLAYER_MARGIN = 10;



Player makeNewPlayer(void) {
    Player p = {
            .score = 0,
            .xPos = 100,
            .yPos = 100,
            .xSpeed = 150.0f,
            .ySpeed = 150.0f,
            .keyboardState = SDL_GetKeyboardState(NULL),
    };
    return p;
}

void updatePlayers(struct Player *players, int numOfPlayers, float elapsed) {
    //Error Check
    if (numOfPlayers < 1) {
        fprintf(stderr, "There must be at least 1 player in the game to start.");
        exit(1);
    }

    //Update for each player
    for (int i = 0; i < numOfPlayers; ++i) {
        //Move Up
        if (players[i].keyboardState[SDL_SCANCODE_W])
        {
            players[i].yPos -= players[i].ySpeed * elapsed;
        }

        //Move Down
        if (players[i].keyboardState[SDL_SCANCODE_S])
        {
            players[i].yPos += players[i].ySpeed  * elapsed;
        }

        //Move Left
        if (players[i].keyboardState[SDL_SCANCODE_A])
        {
            players[i].xPos -= players[i].xSpeed * elapsed;
        }

        //Move Right
        if (players[i].keyboardState[SDL_SCANCODE_D]) {
            players[i].xPos += players[i].xSpeed  * elapsed;
        }

    }
}

void renderPlayers(SDL_Renderer **renderer, struct Player *players, int numOfPlayers) {
    //Error Check
    if (numOfPlayers < 1) {
        fprintf(stderr, "There must be at least 1 player in the game to render.");
        exit(1);
    }


    for (int i = 0; i < numOfPlayers; ++i) {
        SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
        SDL_Rect currentPlayer = {
                .x = (int)players[i].xPos - PLAYER_HEIGHT / 2,
                .y = (int)players[i].yPos - PLAYER_HEIGHT / 2,
                .w = PLAYER_WIDTH,
                .h = PLAYER_HEIGHT
        };
        SDL_RenderFillRect(*renderer, &currentPlayer);
    }
}







