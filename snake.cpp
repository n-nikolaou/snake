#include <iostream>
#include "snake.h"

snake::snake() {
    //ctor
}

void snake::initialize(SDL_Renderer *renderer, int key, food *apple, score *points)
{
    this->apple = apple;
    direction = key;

    length = 1;
    pos = (int**) malloc(sizeof(int*));
    pos[0] = (int*) malloc(2 * sizeof(int));
    pos[0][0] = SCREEN_WIDTH/2 - BODY_SIZE/2;
    pos[0][1] = SCREEN_HEIGHT/2 - BODY_SIZE/2;

    chain = (SDL_Rect**) malloc(sizeof(SDL_Rect*));
    chain[0] = new SDL_Rect;
    chain[0]->h = BODY_SIZE;
    chain[0]->w = BODY_SIZE;
    chain[0]->x = pos[0][0];
    chain[0]->y = pos[0][1];

    this->renderer = renderer;
    this->points = points;
}

void snake::hasEaten(int lastPosTailX, int lastPosTailY)
{
    length++;

    pos = (int**) realloc(pos, length * sizeof(int*));
    pos[length - 1] = (int*) malloc(2 * sizeof(int));

    chain = (SDL_Rect**) realloc(chain, length * sizeof(SDL_Rect*));
    chain[length - 1] = new SDL_Rect;
    chain[length - 1]->w = BODY_SIZE;
    chain[length - 1]->h = BODY_SIZE;

    pos[length - 1][0] = lastPosTailX;
    pos[length - 1][1] = lastPosTailY;

    apple->calcCoords(length, pos);
}

void snake::move(bool *hasStopped, int *dir)
{
    apple->calcCoords(length, pos);
    points->initialize(renderer);
    int lastPosTailX, lastPosTailY, lastDir;
    bool overlapped = false;
    while (!(*hasStopped))
    {
        lastPosTailX = pos[length - 1][0];
        lastPosTailY = pos[length - 1][1];

        if (!overlapped)
        {
            lastDir = direction;
            direction = *dir;
        }
        else overlapped = false;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); //clear everytime the screen

        SDL_SetRenderDrawColor(renderer, 175, 0, 80, 255);
        for (int i = 0; i < EDGE_PX; i++)
        {
            SDL_RenderDrawLine(renderer, 0 + i, 0, 0 + i, SCREEN_HEIGHT);
            SDL_RenderDrawLine(renderer, 0, 0 + i, SCREEN_WIDTH, 0 + i);
            SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT - i, SCREEN_WIDTH, SCREEN_HEIGHT - i);
            SDL_RenderDrawLine(renderer, SCREEN_WIDTH - i, 0, SCREEN_WIDTH - i, SCREEN_HEIGHT);
        }

        for (int i = length - 1; i > 0; i--)
        {
            pos[i][0] = pos[i - 1][0]; // the tail of the snake gets the coords of the previous one and so on
            pos[i][1] = pos[i - 1][1];
        }

        if (length > 1 && (lastDir + direction == SDL_SCANCODE_W + SDL_SCANCODE_S || lastDir + direction == SDL_SCANCODE_A + SDL_SCANCODE_D))
        {
            overlapped = true;
            direction = lastDir;
        }

        if (direction == SDL_SCANCODE_W) pos[0][1] -= STEP; // the head moves by 52pxs
        else if (direction == SDL_SCANCODE_A) pos[0][0] -= STEP;
        else if (direction == SDL_SCANCODE_S) pos[0][1] += STEP;
        else if (direction == SDL_SCANCODE_D) pos[0][0] += STEP;

        if (apple->hasBeenEaten(pos))
        {
            hasEaten(lastPosTailX, lastPosTailY);
            points->increaseValue(length);
        }

        for (int i = 0; i < length; i++)
        {
            chain[i]->x = pos[i][0];
            chain[i]->y = pos[i][1];
            SDL_SetRenderDrawColor(renderer, (R + 2*i >= 245) ? 245 : (R + 2*i), (G + 2*i >= 245) ? 245 : (G + 2*i), (B + 2*i >= 245) ? 245 : (B + 2*i), 255);
            SDL_RenderFillRect(renderer, chain[i]);
            SDL_RenderDrawRect(renderer, chain[i]);
        }

        points->displayScore();

        if (bitItself() != -1 || hasTouchedEdge())
        {
            int block = (bitItself() == -1) ? 0 : bitItself();
            gameOver(block);
            *hasStopped = true;
            return;
        }
        else if (length == apple->getSlots()[0] * apple->getSlots()[1])
        {
            *hasStopped = true;
            return;
        }
        else apple->drawFood();

        SDL_RenderPresent(renderer);
        SDL_Delay(450);
    }
}

bool snake::hasTouchedEdge()
{
    int farL = apple->getEdges()[0], farU = apple->getEdges()[1];
    int horSlots = apple->getSlots()[0], verSlots = apple->getSlots()[1];
    return (pos[0][0] < farL || pos[0][0] > (farL + (verSlots - 1) * STEP) || pos[0][1] < farU || pos[0][1] > (farU + (horSlots - 1) * STEP));
}

void snake::gameOver(int block)
{
    for (int i = 0; i < 5; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 28, 51, 255);
        SDL_RenderFillRect(renderer, chain[block]);
        SDL_RenderDrawRect(renderer, chain[block]);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
        SDL_SetRenderDrawColor(renderer, (R + 2*block >= 245) ? 245 : (R + 2*block), (G + 2*block >= 245) ? 245 : (G + 2*block), (B + 2*block >= 245) ? 245 : (B + 2*block), 255);
        SDL_RenderFillRect(renderer, chain[block]);
        SDL_RenderDrawRect(renderer, chain[block]);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }
}

int snake::bitItself()
{
    for (int i = 1; i < length; i++)
        if (pos[i][0] == pos[0][0] && pos[i][1] == pos[0][1]) return i;

    return -1;
}

void snake::setDirection(int key) {direction = key;;}