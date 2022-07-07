#include <iostream>
#include "../include/input.h"
#include "../include/display.h"
#include "../include/game_manager.h"
#include <SDL2/SDL.h>

void player_keyboard(game& game)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                game.running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    game.running = false;
                }
                else if (event.key.keysym.sym == SDLK_1)
                {
                    game.chooseTower = 1;
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    game.chooseTower = 2;
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    game.chooseTower = 3;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && game.chooseTower == 0 && game.get_money() >= 5)
                {
                    game.select_tower();
                }
                else if (event.button.button == SDL_BUTTON_LEFT && game.chooseTower != 0)
                {
                    game.buy_tower();
                    game.chooseTower = 0;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    game.click_to_upgrade_tower();
                }
            default:
                break;
        }
    }
}