//
// Created by koreansuperfun on 3/28/22.
//

#ifndef COMP4985_GAME_PROTOTYPE_V0_BOOSTS_H
#define COMP4985_GAME_PROTOTYPE_V0_BOOSTS_H

#include <stdbool.h>
#include <stdlib.h>
#include "../Monsters_v2/Monsters_v2.h"


typedef enum {
    INCREASE_BULLET_SPEED,
    SLOW_ENEMY,
    FREEZE,
    KILL_ALL,
    INCREASE_SIZE,
    NUM_OF_BOOSTERS
} Boosters;

typedef struct Boosts {
    float xPos;
    float yPos;
    int red;
    int blue;
    int green;
    int width;
    int height;
    Boosters boosterType;
    bool dropped;
} Boosts;

void dropBoostOnMap(struct Boosts *boosts, int boostPos,
        float xPos, float yPos);

void boost_increaseBulletSpeed(Player *player, float newSpeed, Boosts *boosts, int boostPos);

void boost_slowEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos);
void boost_freezeEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos);
void boost_killAllEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos);
void boost_increaseSizeAllEnemies(struct Monster_v2 *monsters, int numOfMonsters, Boosts *boosts, int boostPos);
void renderAllBoosts(SDL_Renderer **renderer, Boosts *boosts);
Boosts* createAllBoosts();

#endif //COMP4985_GAME_PROTOTYPE_V0_BOOSTS_H
