#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include "../include/enemy.h"
#include "../include/big_enemy.h"
#include "../include/healer.h"
#include "../include/vector.h"
#include "../include/collisions.h"
#include <SDL2/SDL.h>

healer::healer()
{
    damage               = 1;
    healPower            = 1;
    health               = 150;
    definedHealth        = 150;
    definedSpeed         = 160;
    speed                = 160;
    healRange.radCircle  = 130;
    healingSpeed         = 0.1;
    healing              = false;
}

healer::~healer()
{

}

void healer::heal(game& game)
{
    if (timeBetweenHeal > healingSpeed)
    {
        healing = false;
    }
    else
    {
        timeBetweenHeal += game.deltaTime;
    }

    healRange.centerOfCircle = {posPoint.x, posPoint.y};
    
    for (std::unique_ptr<enemy>& enemy : game.enemyList)
    {
        if (enemy->get_alive() == true && enemy->identifier != this->identifier)
        {
            if (interact_with_enemy(enemy))
                return;
        }
    }

    if (healing == false)
    {
        heal_himself();
    }
}

bool healer::interact_with_enemy(std::unique_ptr<enemy>& enemy)
{
    t_point posEnemy = enemy->get_pos();
    if (dotCircle(posEnemy, healRange) && timeBetweenHeal > healingSpeed && healing == false)
    {
        healing = true;
        enemy->take_heal(this->healPower);
        timeBetweenHeal = 0;
        posEnemyLock = posEnemy;
        return 1;
    }
    return 0;  
}

void healer::heal_himself()
{
    healing = true;
    if (health + healPower <= definedHealth)
    {
        health += healPower;
    }
    timeBetweenHeal = 0;
    posEnemyLock = posPoint;
}

void healer::display(const game& game)
{
    if (alive == false)
        return;

    SDL_Renderer* renderer = game.displayM.renderer;

    // display the red rectangles (our enemy)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect firstRect  = {(int)posPoint.x - 6, (int)posPoint.y - 16, 12, 32};
    SDL_Rect secondRect = {(int)posPoint.x - 16, (int)posPoint.y - 6, 32, 12};

    SDL_RenderFillRect(renderer, &firstRect);
    SDL_RenderFillRect(renderer, &secondRect);

    // DISPLAY HEALTH BAR //
    firstRect = {(int)posPoint.x - 24, (int)posPoint.y - 30, 48, 8};
	SDL_RenderFillRect(renderer, &firstRect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    secondRect = {(int)posPoint.x - 24, (int)posPoint.y - 30, (health * 48) / definedHealth, 8};
	SDL_RenderFillRect(renderer, &secondRect);

    // display the "laser" when the healer is healing someone
    if (healing == true)
    {
        SDL_RenderDrawLine(renderer, posPoint.x, posPoint.y, posEnemyLock.x, posEnemyLock.y);
    }
}

void healer::update_enemy(game& game)
{
    display(game);
    move(game);
    heal(game);
}