#include <iostream>
#include <thread>
#include <SDL.h>
#include <SDL_ttf.h>

#include "snake.h"
#include "food.h"
#include "score.h"
#include "display_text.h"

void blinkingText(SDL_Renderer *renderer, bool *flag)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (!(*flag)) {
        for (int i = 0; i < 255; i++) {
            SDL_RenderClear(renderer);
            showText("Press W, A, S or D to start", "C:/Users/nickn/CLionProjects/game/files/font.ttf", 0, 0, 0, i, 20, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, renderer);
            SDL_Delay(5);
        }
        for (int i = 0; i < 255; i++) {
            SDL_RenderClear(renderer);
            showText("Press W, A, S or D to start", "C:/Users/nickn/CLionProjects/game/files/font.ttf", 0, 0, 0, 255 - i, 20, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, renderer);
            SDL_Delay(5);
        }
    }
}

void callMove(snake entity, bool *hasStopped, int *direction)
{
    while (!(*direction))
        continue;

    //these commands get executed when the loop ends using the direction flag
    entity.setDirection(*direction);
    entity.move(hasStopped, direction);
}

int main(int argc, char* args[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) //initialize sdl
    {
        std::cout<<"Failed to initialize SDL: "<<SDL_GetError<<std::endl;
        return -1;
    }

    if(TTF_Init()) std::cout<<TTF_GetError<<std::endl;

    //create window
    SDL_Window *window = SDL_CreateWindow("Basic Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0); //create renderer

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//set color bg to white
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool isRunning = 1, keyboardInput = 1, started = 0, hasStopped = 1;

    std::thread first(blinkingText, renderer, &started); //blinking text functioning with 2nd thread

    int direction = 0;
    snake entity;
    food apple;
    score points;
    entity.initialize(renderer, direction, &apple, &points);
    apple.initialize(renderer, &entity);

    std::thread movement(callMove, entity, &hasStopped, &direction); //getting ready the snake

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
            switch (event.type)
            {
                case SDL_QUIT:
                    isRunning = 0;
                    movement.join();
                case SDL_KEYDOWN:
                    if (!started && (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_S || event.key.keysym.scancode == SDL_SCANCODE_D))
                    {
                        started = 1;
                        first.join(); //blinking text stops

                        direction = event.key.keysym.scancode;
                        hasStopped = 0; //flag changes so the movement starts
                    }
                    //recognize only these movements while the game is running
                    else if (started && (event.key.keysym.scancode == SDL_SCANCODE_W)) direction = event.key.keysym.scancode;
                    else if (started && (event.key.keysym.scancode == SDL_SCANCODE_A)) direction = event.key.keysym.scancode;
                    else if (started && (event.key.keysym.scancode == SDL_SCANCODE_S)) direction = event.key.keysym.scancode;
                    else if (started && (event.key.keysym.scancode == SDL_SCANCODE_D)) direction = event.key.keysym.scancode;

                default:
                    break;
            }
    }

    return 0;
}
