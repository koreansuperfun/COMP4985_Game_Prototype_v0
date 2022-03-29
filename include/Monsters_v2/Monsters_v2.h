#ifndef COMP4985_GAME_PROTOTYPE_V0_MONSTERS_V2_H
#define COMP4985_GAME_PROTOTYPE_V0_MONSTERS_V2_H

#include <stdbool.h>
#include "../Players/Bullets.h"
#include <SDL2/SDL.h>
#include "../Players/Players.h"


typedef struct Monster_v2{
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    float width;
    float height;
    float timeBetweenShots;
    int defaultNumOfBullets;
    float bulletSpeed;
    bool alive;
    Bullet *listOfBullets;
    float timeSinceLastShot;
    Player *playerToChase;
} Monster_v2;

void renderAllMonsters(SDL_Renderer **renderer, struct Monster_v2 *monsters, int numOfMonsters);

Monster_v2* createArrayOfMonstersV2(int numOfMonsters);

void updateAllMonsters(float elapsed, Monster_v2 *arrayOfMonsters,
                       int numOfMonsters,int mapWidth, int mapHeight
        , int xOffset, int yOffset);

void summonMonsters(struct Monster_v2 *monsters, Player *playerToChase,
                    int numberOfMonsters,int xMinPos, int xMaxPos,
                    int yMinPos, int yMaxPos);

#endif //COMP4985_GAME_PROTOTYPE_V0_MONSTERS_V2_H
