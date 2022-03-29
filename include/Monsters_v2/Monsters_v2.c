//
// Created by koreansuperfun on 3/28/22.
//

#include "Monsters_v2.h"
#include <SDL2/SDL.h>
#include <time.h>


const float MONSTER_DEFAULT_HEIGHT = 70;
const float MONSTER_DEFAULT_WIDTH = 40;
const int DEFAULT_NUM_BULLETS = 10;
const float DEFAULT_TIME_BETWEEN_SHOT = 10.0f;
const float DEFAULT_MONSTER_SPEED = 150.0f;
const float DEFAULT_MONSTER_BULLET_SPEED = 500.0f;
const int BULLET_SIZE_MONSTER = 10;

int getRandomNumberGeneratorV2(int lower, int upper) {
    srand(time(0));
    return (rand() % (upper - lower + 1) + lower);

}

Monster_v2* createArrayOfMonstersV2(int numOfMonsters) {
    Monster_v2 *monsters = (Monster_v2*) malloc(numOfMonsters * sizeof(Monster_v2));
    for (int i = 0; i < numOfMonsters; ++i) {
        monsters[i].xPos = 0;
        monsters[i].yPos = 0;
        monsters[i].xSpeed = DEFAULT_MONSTER_SPEED;
        monsters[i].ySpeed = DEFAULT_MONSTER_SPEED;
        monsters[i].width = MONSTER_DEFAULT_WIDTH;
        monsters[i].height = MONSTER_DEFAULT_HEIGHT;
        monsters[i].timeBetweenShots = DEFAULT_TIME_BETWEEN_SHOT;
        monsters[i].defaultNumOfBullets = DEFAULT_NUM_BULLETS;
        monsters[i].bulletSpeed = DEFAULT_MONSTER_BULLET_SPEED;
        monsters[i].listOfBullets = (Bullet*) malloc(DEFAULT_NUM_BULLETS * sizeof(Bullet));
        monsters[i].alive = false;
        monsters[i].timeSinceLastShot = 0;
        monsters[i].playerToChase = NULL;
        for (int j = 0; j < monsters[i].defaultNumOfBullets; ++j) {
            monsters[i].listOfBullets[i].live = false;
        }
    }
    return monsters;
}

void renderAllMonsters(SDL_Renderer **renderer, struct Monster_v2 *monsters, int numOfMonsters){
    for (int i = 0; i < numOfMonsters; ++i){
        if (monsters[i].alive == true) {
            SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
            SDL_Rect currentMonster = {
                    .x = (int)monsters[i].xPos,
                    .y = (int)monsters[i].yPos,
                    .w = (int)monsters[i].width,
                    .h = (int)monsters[i].height,
            };
            SDL_RenderFillRect(*renderer, &currentMonster);

            //Render Each bullet
            for (int j = 0; j < monsters[i].defaultNumOfBullets; ++j) {
                if (monsters[i].listOfBullets[j].live == true) {
                    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
                    SDL_Rect currentBullet = {
                            .x = (int) monsters[i].listOfBullets[j].xPos,
                            .y = (int) monsters[i].listOfBullets[j].yPos,
                            .w = BULLET_SIZE_MONSTER,
                            .h = BULLET_SIZE_MONSTER
                    };
                    SDL_RenderFillRect(*renderer, &currentBullet);
                }
            }
        }
    }
}

void updateAllMonsters(float elapsed, Monster_v2 *arrayOfMonsters,
                    int numOfMonsters,int mapWidth, int mapHeight
        , int xOffset, int yOffset) {
    for (int i = 0; i < numOfMonsters; ++i) {
        if (arrayOfMonsters[i].alive == true) {
            float playerXPos = arrayOfMonsters[i].playerToChase->xPos;
            float playerYPos = arrayOfMonsters[i].playerToChase->yPos;
            float xDifference = playerXPos - arrayOfMonsters[i].xPos;
            float yDifference = playerYPos - arrayOfMonsters[i].yPos;

            //Set Monster Movement
            if (xDifference > 0) {
                arrayOfMonsters[i].xPos += arrayOfMonsters[i].xSpeed * elapsed;
            } else {
                arrayOfMonsters[i].xPos -= arrayOfMonsters[i].xSpeed * elapsed;
            }

            if (yDifference > 0) {
                arrayOfMonsters[i].yPos += arrayOfMonsters[i].ySpeed * elapsed;
            } else {
                arrayOfMonsters[i].yPos -= arrayOfMonsters[i].ySpeed * elapsed;
            }

            /**
             * Update Each bullet position
             */
            for (int j = 0; j < arrayOfMonsters[i].defaultNumOfBullets; ++j) {
                if (arrayOfMonsters[i].listOfBullets[j].live == true) {
                    arrayOfMonsters[i].listOfBullets[j].xPos += arrayOfMonsters[i].listOfBullets[j].xSpeed * elapsed;
                    arrayOfMonsters[i].listOfBullets[j].yPos += arrayOfMonsters[i].listOfBullets[j].ySpeed * elapsed;

                    if ((int) arrayOfMonsters[i].listOfBullets[j].xPos < xOffset || (int) arrayOfMonsters[i].listOfBullets[j].xPos > (xOffset + mapWidth)
                        || (int) arrayOfMonsters[i].listOfBullets[j].yPos < yOffset || (int) arrayOfMonsters[i].listOfBullets[j].yPos > (yOffset + mapHeight)) {
                        arrayOfMonsters[i].listOfBullets[j].live = false;
                    }
                }
            }


            /**
             * Check when was the last time bullet was shot by each monster. If it's been over the limit set,
             * then shoot another bullet.
             */
            arrayOfMonsters[i].timeSinceLastShot += elapsed;
            if (arrayOfMonsters[i].timeSinceLastShot > arrayOfMonsters[i].timeBetweenShots) {
                for (int j = 0; j < arrayOfMonsters[i].defaultNumOfBullets; ++j) {
                    if (arrayOfMonsters[i].listOfBullets[j].live == false) {
                        arrayOfMonsters[i].listOfBullets[j].live = true;
                        arrayOfMonsters[i].listOfBullets[j].xPos = arrayOfMonsters[i].xPos;
                        arrayOfMonsters[i].listOfBullets[j].yPos = arrayOfMonsters[i].yPos;

                        //Does the match to set the correct x/y speed of the bullet.
                        float totalDistance = sqrtf(((arrayOfMonsters[i].xPos - arrayOfMonsters[i].playerToChase->xPos) * (arrayOfMonsters[i].xPos - arrayOfMonsters[i].playerToChase->xPos))
                                                    + ((arrayOfMonsters[i].yPos - arrayOfMonsters[i].playerToChase->yPos) * (arrayOfMonsters[i].yPos - arrayOfMonsters[i].playerToChase->yPos)));
                        float speedFactor = totalDistance / arrayOfMonsters[i].bulletSpeed;
                        float xSpeedToAdd = (arrayOfMonsters[i].playerToChase->xPos - arrayOfMonsters[i].xPos) / speedFactor;
                        float ySpeedToAdd = (arrayOfMonsters[i].playerToChase->yPos - arrayOfMonsters[i].yPos) / speedFactor;
                        arrayOfMonsters[i].listOfBullets[i].xSpeed =  xSpeedToAdd;
                        arrayOfMonsters[i].listOfBullets[i].ySpeed =  ySpeedToAdd;


                        arrayOfMonsters[i].timeSinceLastShot -= arrayOfMonsters[i].timeBetweenShots;

                        //To break out of the loop
//                        j = arrayOfMonsters[i].defaultNumOfBullets;
                        break;
                    }
                }
            }
        }
    }
}

void summonMonsters(struct Monster_v2 *monsters, Player *playerToChase,
        int numberOfMonsters,int xMinPos, int xMaxPos,
                    int yMinPos, int yMaxPos) {
    printf("Summoned Monstesr here!\n");
    for (int i = 0; i < numberOfMonsters; ++i) {
        if (monsters[i].alive == false) {
            monsters[i].alive = true;
            monsters[i].xPos = (float) getRandomNumberGeneratorV2(xMinPos, xMaxPos);
            monsters[i].yPos = (float) getRandomNumberGeneratorV2(yMinPos, yMaxPos);
            monsters[i].playerToChase = playerToChase;
            monsters[i].xSpeed = DEFAULT_MONSTER_SPEED;
            monsters[i].ySpeed = DEFAULT_MONSTER_SPEED;
            monsters[i].width = MONSTER_DEFAULT_WIDTH;
            monsters[i].height = MONSTER_DEFAULT_HEIGHT;
            return;
        }
    }
}