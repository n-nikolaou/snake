#ifndef GAME_DISPLAY_TEXT_H
#define GAME_DISPLAY_TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

inline void showText(char *script, char *fontFile, int r, int g, int b, int a, int size, int x, int y, SDL_Renderer *renderer)
{
    SDL_Color textColor;
    textColor.a = a;
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;

    TTF_Font *font = TTF_OpenFont(fontFile, size);

    int wText, hText;
    TTF_SizeText(font, script, &wText, &hText);
    SDL_Rect srcText;
    srcText.x = 0;
    srcText.y = 0;
    srcText.w = wText;
    srcText.h = hText;

    SDL_Rect dstText = srcText;
    dstText.x = x - wText/2;
    dstText.y = y - hText/2;

    SDL_Surface *text = TTF_RenderText_Solid(font, script, textColor); //creates the text
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text); //transform to texture from surface
    SDL_RenderCopy(renderer, textTexture, &srcText, &dstText); //render it
    SDL_RenderPresent(renderer); //present it
    TTF_CloseFont(font);
}

#endif //GAME_DISPLAY_TEXT_H
