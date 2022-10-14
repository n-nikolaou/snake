#include <iostream>
#include "snake.h"

snake::snake() {
    //ctor
}

void snake::initialize(SDL_Renderer *renderer, int key, food *apple, score *points)
{
    direction = key;

    //pos represents the coords of every "body part" of the snake
    length = 1;
    pos = (int**) malloc(sizeof(int*));
    pos[0] = (int*) malloc(2 * sizeof(int));
    pos[0][0] = SCREEN_WIDTH/2 - BODY_SIZE/2;
    pos[0][1] = SCREEN_HEIGHT/2 - BODY_SIZE/2;

    //chain represents the graphics of every body part of the snake
    chain = (SDL_Rect**) malloc(sizeof(SDL_Rect*));
    chain[0] = new SDL_Rect;
    chain[0]->h = BODY_SIZE;
    chain[0]->w = BODY_SIZE;
    chain[0]->x = pos[0][0];
    chain[0]->y = pos[0][1];

    this->apple = apple;
    this->renderer = renderer;
    this->points = points;
}

//functions when the snake has eaten an apple
void snake::hasEaten(int lastPosTailX, int lastPosTailY)
{
    length++;

    //lengthen the pos and chain table
    pos = (int**) realloc(pos, length * sizeof(int*));
    pos[length - 1] = (int*) malloc(2 * sizeof(int));

    chain = (SDL_Rect**) realloc(chain, length * sizeof(SDL_Rect*));
    chain[length - 1] = new SDL_Rect;
    chain[length - 1]->w = BODY_SIZE;
    chain[length - 1]->h = BODY_SIZE;

    //the new body part generating gets the coords of the previous tail of the snake
    pos[length - 1][0] = lastPosTailX;
    pos[length - 1][1] = lastPosTailY;

    //re-calculate the coords of the new apple
    apple->calcCoords(length, pos);
}

//the main part of the snake
//to be reconstructed
void snake::move(bool *hasStopped, int *dir)
{
    apple->calcCoords(length, pos);
    //initialize the score system
    points->initialize(renderer);

    int lastPosTailX, lastPosTailY, lastDir;
    bool overlapped = false;
    while (!(*hasStopped))
    {
        //the tail of the snake gets the coords of the previous tail
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

        //draw the edges of the screen
        SDL_SetRenderDrawColor(renderer, 175, 0, 80, 255);
        for (int i = 0; i < EDGE_PX; i++)
        {
            SDL_RenderDrawLine(renderer, 0 + i, 0, 0 + i, SCREEN_HEIGHT);
            SDL_RenderDrawLine(renderer, 0, 0 + i, SCREEN_WIDTH, 0 + i);
            SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT - i, SCREEN_WIDTH, SCREEN_HEIGHT - i);
            SDL_RenderDrawLine(renderer, SCREEN_WIDTH - i, 0, SCREEN_WIDTH - i, SCREEN_HEIGHT);
        }

        //every i-body part gets the coords of the (i-1)-body part
        for (int i = length - 1; i > 0; i--)
        {
            pos[i][0] = pos[i - 1][0];
            pos[i][1] = pos[i - 1][1];
        }

        //checks if the snake tried to cross over itself
        //ie: if the snake is moving south and you try to make it move north
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

        //draw every body part of the snake
        for (int i = 0; i < length; i++)
        {
            chain[i]->x = pos[i][0];
            chain[i]->y = pos[i][1];
            SDL_SetRenderDrawColor(renderer, (R + 2*i >= 245) ? 245 : (R + 2*i), (G + 2*i >= 245) ? 245 : (G + 2*i), (B + 2*i >= 245) ? 245 : (B + 2*i), 255);
            SDL_RenderFillRect(renderer, chain[i]);
            SDL_RenderDrawRect(renderer, chain[i]);
        }

        points->displayScore();

        //conditions when the game is over
        if (bitItself() != -1 || hasTouchedEdge())
        {
            int block = (bitItself() == -1) ? 0 : bitItself();
            gameOver(block);
            *hasStopped = true;
            return;
        }
        //condition when the snake has filled the screen
        else if (length == apple->getSlots()[0] * apple->getSlots()[1])
        {
            *hasStopped = true;
            return;
        }
        //if the game continues we draw the food
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
    //graphics of the body part eaten or the head touching the edge
    //creates a red flashing on the body part
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