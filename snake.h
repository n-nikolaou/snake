#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H

#include <SDL.h>
#include <cstdlib>
#include <thread>

#include "food.h"
#include "score.h"

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 720
#define BODY_SIZE 50
#define STEP (BODY_SIZE + 2)
#define EDGE_PX 6

#define R 52
#define G 150
#define B 88

class food;
class score;
class snake {
private:
    int length;
    int **pos = nullptr;
    SDL_Rect **chain = nullptr;
    SDL_Renderer *renderer;
    int direction;
    food *apple;
    score *points;

public:
    void initialize(SDL_Renderer*, int, food*, score*);
    snake();

    void hasEaten(int, int);
    void move(bool*, int*);

    int bitItself();
    bool hasTouchedEdge();
    void gameOver(int);

    void setDirection(int);
};

#endif //GAME_SNAKE_H