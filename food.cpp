#include "food.h"

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
#include <iostream>
bool food::hasCollided(int x, int y, int length, int **pos)
{
    for (int i = 0; i < length; i++)
        if (pos[i][0] == x && pos[i][1] == y) return true;

    return false;
}

bool food::hasBeenEaten(int **pos)
{
    while (pos[0] == nullptr)
        continue;

    return (pos[0][0] == coords[0] && pos[0][1] == coords[1]);
}

food::food()
{
    //ctor
}

void food::initialize(SDL_Renderer *renderer, snake *entity) {
    coords = (int*) malloc(2 * sizeof(int));
    this->entity = entity;
    this->renderer = renderer;

    int centerH = SCREEN_HEIGHT/2 - BODY_SIZE/2;
    int centerW = SCREEN_WIDTH/2 - BODY_SIZE/2;

    int tempH = centerH, tempW = centerW;
    int i;
    for (i = 0; tempW > 0; i++)
        tempW -= STEP;
    farL = tempW + STEP;

    tempW = centerW + STEP;
    for (; tempW + STEP < SCREEN_WIDTH; i++)
        tempW += STEP;

    int j;
    for (j = 0; tempH > 0; j++)
        tempH -= STEP;
    farU = tempH + STEP;

    tempH = centerH + STEP;
    for (; tempH + STEP < SCREEN_HEIGHT; j++)
        tempH += STEP;

    horSlots = i;
    verSlots = j;
}

void food::calcCoords(int length, int **pos)
{
    int randX = (rand() % verSlots) * STEP + farL, randY = (rand() % horSlots) * STEP + farU;
    while (hasCollided(randX, randY, length, pos)) {
        randX += STEP;
        if (randX == verSlots * STEP + farL)
        {
            randY += STEP;
            randX = farL;
        }
        if (randY == horSlots * STEP + farU) randY = farU;
    }

    coords[0] = randX;
    coords[1] = randY;
}

void food::drawFood()
{
    SDL_SetRenderDrawColor(renderer, 255, 128, 179, 255);
    for (int i = RADIUS; i > 0; i--)
      DrawCircle(renderer, coords[0] + BODY_SIZE/2, coords[1] + BODY_SIZE/2, i);
}

int* food::getEdges()
{
    int *table = (int*) malloc(2 * sizeof(int));
    table[0] = farL;
    table[1] = farU;
    return table;
}

int* food::getSlots()
{
    int *table = (int*) malloc(2 * sizeof(int));
    table[0] = horSlots;
    table[1] = verSlots;
    return table;
}