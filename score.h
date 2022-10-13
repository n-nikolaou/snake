#ifndef GAME_SCORE_H
#define GAME_SCORE_H

#include <SDL_ttf.h>
#include <cstring>

#include "snake.h"
#include "display_text.h"

class snake;
class score {
private:
    int value;
    SDL_Renderer *renderer;
    char *text;

public:
    score();
    void initialize(SDL_Renderer*);

    void increaseValue(int);
    void displayScore();
};


#endif //GAME_SCORE_H
