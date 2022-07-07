#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include "../include/enemy.h"
#include "../include/big_enemy.h"
#include "../include/vector.h"
#include <SDL2/SDL.h>

big_enemy::big_enemy()
{
    damage          = 5;
    health          = 1000;
    definedHealth   = 1000;
    definedSpeed    = 80;
    speed           = 80;
}

big_enemy::~big_enemy()
{

}

void big_enemy::display(const game& game)
{
    if (alive == false)
        return;

    SDL_Renderer* renderer = game.displayM.renderer;

    // display the red rectangle (our enemy)
    int x = posPoint.x;
    int y = posPoint.y;

    SDL_SetRenderDrawColor(renderer, 8, 129, 223, 255);
    SDL_Rect rect = {x - 16, y - 16, 32, 32};
	SDL_RenderFillRect(renderer, &rect);
    

    // DISPLAY HEALTH BAR //
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    rect = {x - 24, y - 30, 48, 8};
	SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    rect = {x - 24, y - 30, (health * 48) / definedHealth, 8};
	SDL_RenderFillRect(renderer, &rect);
}