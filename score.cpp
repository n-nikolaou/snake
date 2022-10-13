#include "score.h"

score::score() {
    //ctor
}

void score::initialize(SDL_Renderer *renderer)
{
    value = 0;
    this->renderer = renderer;
}

void score::increaseValue(int length)
{
    value++;
}

void score::displayScore()
{
    text = (char*) malloc(20 * sizeof(char));
    strcpy(text, "Score: \0");
    char *num = (char*) malloc(3 * sizeof(char));
    sprintf(num, "%d\0", value);
    strcat(text, num);
    showText(text, "C:/Users/nickn/CLionProjects/game/files/font.ttf", 0, 0, 0, 255, 30, SCREEN_WIDTH/2, EDGE_PX + 15, renderer);
}