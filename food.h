#ifndef GAME_FOOD_H
#define GAME_FOOD_H

#include "snake.h"
#include <time.h>
#include <random>

#define RADIUS 20

class snake;
class food {
private:
    snake *entity;
    SDL_Renderer *renderer;
    int horSlots, verSlots;
    int farL, farU;
    int *coords;

public:
    food();

    void initialize(SDL_Renderer*, snake*);
    void drawFood();
    void calcCoords(int, int**);

    bool hasBeenEaten(int**);
    bool hasCollided(int, int, int, int**);
    int* getEdges();
    int* getSlots();

};

#endif //GAME_FOOD_H