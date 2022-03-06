//
// Created by koreansuperfun on 3/5/22.
//

#include "Players.h"

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;
const int PLAYER_MARGIN = 10;

typedef struct Player {
    int score;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
} Player;

Player makeNewPlayer() {
    Player p = {
            .score = 0,
            .xPos = 100,
            .yPos = 100,

    };
}






