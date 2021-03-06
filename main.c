#include <stdio.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "include/State/state.h"
#include "include/Players/Players.h"
#include "include/Maps/Maps.h"
#include "include/Monsters/Monsters.h"
#include "include/Monsters_v2/Monsters_v2.h"
#include "include/Boosts/Boosts.h"


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


//Monster Section
Monster *monsters;
const int NUM_OF_MONSTERS = 1;
const int CHANCE_OF_MONSTER_SPAWN = 100;

Monster_v2 *monsterV2;
const int NUM_OF_MONSTERS_V2 = 10;
const float SPAWN_EVERY_FLOAT = 1.0f;
float timeSinceLastMonsterSpawn = 0;
void updateMonsterTimeLimit(float elapsed) {
    timeSinceLastMonsterSpawn += elapsed;
}

//Boost Section
Boosts *boosts;
int TEST_numOfBoostsDropped = 0;

//GameMaster
/*
 * This should really be a class of its own called GameMaster.
 * But for now, we just need one function for the prototype.
 */
const int MAX_SCORE_LIMIT = 1500;
void dropBoost(float xPos, float yPos) {
    if (TEST_numOfBoostsDropped < NUM_OF_BOOSTERS) {
        dropBoostOnMap(boosts, TEST_numOfBoostsDropped++,
                       xPos, yPos);
    }
}
void check_if_bullet_struck_monster() {
    for (int i = 0; i < player1.maxNumOfBullets; ++i) {
        for (int j = 0; j < NUM_OF_MONSTERS_V2; ++j) {
            if (monsterV2[j].alive == true && player1.listOfBullets[i].live == true
                &&player1.listOfBullets[i].xPos > monsterV2[j].xPos
                && player1.listOfBullets[i].xPos < monsterV2[j].xPos + monsterV2[j].width
                && player1.listOfBullets[i].yPos > monsterV2[j].yPos
                &&player1.listOfBullets[i].yPos < monsterV2[j].yPos + monsterV2[j].height) {

                    dropBoost(monsterV2[j].xPos, monsterV2[j].yPos);

                    player1.score += 1;
                    printf("Hit detected! Player score: %d\n", player1.score);
                    monsterV2[j].alive = false;
                    monsterV2[j].xPos = 0;
                    monsterV2[j].yPos = 0;
                    player1.listOfBullets[i].live = false;
                    return;
            }
        }

    }
}

void check_if_player_got_booster() {
    for (int i = 0; i < NUM_OF_BOOSTERS; ++i) {
        if (boosts[i].dropped == true
            &&player1.xPos > boosts[i].xPos - (float) boosts[i].width
            && player1.xPos < boosts[i].xPos + ((float) boosts[i].width * 2)
            && player1.yPos > boosts[i].yPos  - (float) boosts[i].height
            &&player1.yPos < boosts[i].yPos + ((float) boosts[i].height * 2)) {
            switch(i) {
                case INCREASE_BULLET_SPEED:
                    boost_increaseBulletSpeed(&player1, 800.0f, boosts, i);
                    break;
                case SLOW_ENEMY:
                    boost_slowEnemies(monsterV2, NUM_OF_MONSTERS_V2, boosts, i);
                    break;
                case FREEZE:
                    boost_freezeEnemies(monsterV2, NUM_OF_MONSTERS_V2, boosts, i);
                    break;
                case KILL_ALL:
                    boost_killAllEnemies(monsterV2, NUM_OF_MONSTERS_V2, boosts, i);
                    break;
                case INCREASE_SIZE:
                    boost_increaseSizeAllEnemies(monsterV2, NUM_OF_MONSTERS_V2, boosts, i);

                    break;
                default:
                    break;
            }

        }
    }
}

int main() {
    SDL_Event event;
    uint32_t lastTick;
    atexit(shutdown);
    enum game_state current_state;
    bool quit;
    float totalElapsedTime = 0;

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
                break;
            case MAIN_GAME_STATE:
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }
                    switch(event.type){
                        case SDL_QUIT:
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            addNewBullet(player1, event.button.x, event.button.y);
                            break;
                    }
                }
                uint32_t curTick = SDL_GetTicks();
                uint32_t diff = curTick - lastTick;
                float elapsed = (float) diff / 1000.0f;
                update(elapsed);
                lastTick = curTick;
                totalElapsedTime += elapsed;

                /*
                 * This should really be a state of its own, but I'm too lazy.
                 * TODO:Turn this into an FSM.
                 */
                if (player1.score == MAX_SCORE_LIMIT) {
                    printf("Congratulations on hitting the monster %d times!\n", MAX_SCORE_LIMIT);
                    printf("Your total time is %f\n", totalElapsedTime);
                    player1.score = 0;
                    totalElapsedTime= 0;
                    //Prob should go into another state, but this is good enough for prototype.
                    current_state = INIT_STATE;
                }
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

    //Proof of concept for making borderlines.
    maps = makeMap(MAP_DEFAULT_HEIGHT, MAP_DEFAULT_WIDTH, map_array);
    setBorderLine(&maps,100, 100, MAP_DEFAULT_WIDTH - 200, 'r');
    setBorderLine(&maps,100, 100, 550, 'd');
    setBorderLine(&maps,1500, 650, 550, 'u');
    setBorderLine(&maps,1500, 650, MAP_DEFAULT_WIDTH - 200, 'l');

    player1 = makeNewPlayer(maps.width / 2, maps.height / 2);
    monsters = createArrayOfMonsters(NUM_OF_MONSTERS);
    monsterV2 = createArrayOfMonstersV2(NUM_OF_MONSTERS_V2);
    boosts = createAllBoosts();

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        printf("arrived here1\n");

        if (SDL_IsGameController(i)) {
            printf("arrived here2\n");

            controller = SDL_GameControllerOpen(i);
            if (controller) {
                printf("Controller found here at %d\n", i);
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }

    return true;
}

void update(float elapsed) {
    SDL_SetRenderDrawColor(renderer, 128, 192, 255, 255);
    SDL_RenderClear(renderer);
    check_if_bullet_struck_monster();
    check_if_player_got_booster();

    renderMap(&renderer, maps, SCREEN_HEIGHT, SCREEN_WIDTH);
    int xOffSetToCenter = (SCREEN_WIDTH / 2) - (maps.width / 2);
    int yOffSetToCenter = (SCREEN_HEIGHT / 2) - (maps.height / 2);
    updatePlayers(&player1, numOfPlayers, elapsed, map_array,
                  MAP_DEFAULT_WIDTH, MAP_DEFAULT_HEIGHT,
                  xOffSetToCenter, yOffSetToCenter);
    renderPlayers(&renderer, &player1, 1);
//    randomlyGenerateMonster(monsters, CHANCE_OF_MONSTER_SPAWN, NUM_OF_MONSTERS,
//                            xOffSetToCenter, SCREEN_WIDTH - xOffSetToCenter,
//                            yOffSetToCenter, SCREEN_HEIGHT - yOffSetToCenter);
//    updateMonstersLifeDuration(elapsed, monsters, NUM_OF_MONSTERS);
//
//    renderMonsters(&renderer, monsters, NUM_OF_MONSTERS);


    /**Render All Boosts*/
    renderAllBoosts(&renderer, boosts);

    /**Render All Monsters*/
    updateMonsterTimeLimit(elapsed);
    if (timeSinceLastMonsterSpawn > SPAWN_EVERY_FLOAT) {
        timeSinceLastMonsterSpawn -= SPAWN_EVERY_FLOAT;
        summonMonsters(monsterV2, &player1, NUM_OF_MONSTERS_V2, xOffSetToCenter, SCREEN_WIDTH - xOffSetToCenter,
                            yOffSetToCenter, SCREEN_HEIGHT - yOffSetToCenter);
    }
    updateAllMonsters(elapsed, monsterV2, NUM_OF_MONSTERS_V2,MAP_DEFAULT_WIDTH, MAP_DEFAULT_HEIGHT,
                      xOffSetToCenter, yOffSetToCenter);
    renderAllMonsters(&renderer, monsterV2, NUM_OF_MONSTERS_V2);
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

