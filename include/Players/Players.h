#ifndef COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H
#define COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H


typedef struct Player{
    int score;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    const Uint8 *keyboardState;
} Player;

Player makeNewPlayer(int startingPosX, int startingPosY);
void updatePlayers(struct Player *players, int numOfPlayers, float elapsed);
void renderPlayers(SDL_Renderer **renderer, struct Player *players, int numOfPlayers);


#endif //COMP4985_GAME_PROTOTYPE_V0_PLAYERS_H
