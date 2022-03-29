//
// Created by koreansuperfun on 3/28/22.
//

#include "Boosts.h"

const int boost_size = 20;

Boosts* createAllBoosts() {
    Boosts *boosts = (Boosts *) malloc(NUM_OF_BOOSTERS * sizeof(Boosts));
    for (int i = 0; i < NUM_OF_BOOSTERS; ++i) {
        boosts[i].xPos = 0;
        boosts[i].yPos = 0;
        boosts[i].boosterType = i;
        boosts[i].dropped = false;
        boosts[i].width = boost_size;
        boosts[i].height = boost_size;
        switch(i) {
            case INCREASE_BULLET_SPEED:
                boosts[i].red = 255;
                boosts[i].blue = 51;
                boosts[i].green = 255;
                break;
            case SLOW_ENEMY:
                boosts[i].red = 0;
                boosts[i].blue = 0;
                boosts[i].green = 204;
                break;
            case FREEZE:
                boosts[i].red = 51;
                boosts[i].blue = 255;
                boosts[i].green = 255;
                break;
            case KILL_ALL:
                boosts[i].red = 255;
                boosts[i].blue = 0;
                boosts[i].green = 0;
                break;
            case INCREASE_SIZE:
                boosts[i].red = 102;
                boosts[i].blue = 0;
                boosts[i].green = 51;
                break;
            default:
                break;
        }
    }
    return boosts;
}

void dropBoostOnMap(struct Boosts *boosts, int boostPos, float xPos, float yPos) {
    boosts[boostPos].xPos = xPos;
    boosts[boostPos].yPos = yPos;
    boosts[boostPos].dropped = true;
}

void boost_increaseBulletSpeed(Player *player, float newSpeed, Boosts *boosts, int boostPos) {
    player->bulletSpeed = newSpeed;
    boosts[boostPos].dropped = false;
}

void boost_slowEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (monsters[i].alive == true) {
            monsters[i].xSpeed /= 2;
            monsters[i].ySpeed /= 2;
        }
    }
    boosts[boostPos].dropped = false;

}

void boost_freezeEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (monsters[i].alive == true) {
            monsters[i].xSpeed = 0;
            monsters[i].ySpeed = 0;
        }
    }
    boosts[boostPos].dropped = false;

}

void boost_killAllEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (monsters[i].alive == true) {
            monsters[i].alive = false;
        }
    }
    boosts[boostPos].dropped = false;
}

void boost_increaseSizeAllEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (monsters[i].alive == true) {
            monsters[i].width *= 2;
            monsters[i].height *= 2;
        }
    }
    boosts[boostPos].dropped = false;
}

void renderAllBoosts(SDL_Renderer **renderer, Boosts *boosts) {
    for (int i = 0; i < NUM_OF_BOOSTERS; ++i) {
        if (boosts[i].dropped == true) {
            SDL_SetRenderDrawColor(*renderer, boosts[i].red, boosts[i].green,
                                   boosts[i].blue, 255);
            SDL_Rect currentBoost = {
                    .x = (int)boosts[i].xPos,
                    .y = (int)boosts[i].yPos,
                    .w = boost_size,
                    .h = boost_size,
            };
            SDL_RenderFillRect(*renderer, &currentBoost);
        }
    }
}
