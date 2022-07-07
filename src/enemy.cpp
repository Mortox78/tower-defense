#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include "../include/enemy.h"
#include "../include/vector.h"
#include <SDL2/SDL.h>

enemy::enemy()
{
    damage                       = 2;
    health                       = 300;
    definedHealth                = 300;
    vecMove                      = {1, 0};
    posPoint                     = {-32, 352};
    alive                        = false;
    appeared                     = false;
    slowed                       = false;
    definedSpeed                 = 120;
    speed                        = 120;
    slowTime                     = 0;
    counterPattern               = 0;
    circleEnemyDisplay.radCircle = 16;
    init_movement_pattern();
}

enemy::~enemy()
{

}

void enemy::move(game& game)
{
    if (alive == false)
        return;
    
    // manage the slow down given by the slow tower 
    manage_slow(game);
    
    // then move
    vecMove   = getVectorFromDots(pattern[counterPattern], pattern[counterPattern + 1]);
    vecMove   = unitaryVector(vecMove);
    posPoint.x += vecMove.x * speed * game.deltaTime;
    posPoint.y += vecMove.y * speed * game.deltaTime;

    // if hit the castle, the castle loses HP and our enemy dies
    hit_castle(game);

    // if our enemy reach a point of our pattern, he will now go to the next point
    if (reach_point_pattern())  
    {
        ++counterPattern;
    }
}

int enemy::get_health() const
{
    return health;
}

void enemy::take_damage(const int& damageReceived, game& game)
{
    health -= damageReceived;

    if (health <= 0)
    {
        die();
        game.earn_money(5);
    }
}

void enemy::die()
{
    alive      = false;
    posPoint.x = -100;
    posPoint.y = -100;
}

bool enemy::get_alive() const
{
    return alive;
}

void enemy::spawn()
{
    alive    = true;
    appeared = true;
}

void enemy::init_movement_pattern()
{
    pattern = {
    {-32, 352},
    {3  * 64 + 32, 5 * 64 + 32},
    {3  * 64 + 32, 2 * 64 + 32},
    {8  * 64 + 32, 2 * 64 + 32},
    {8  * 64 + 32, 5 * 64 + 32},
    {5  * 64 + 32, 5 * 64 + 32},
    {5  * 64 + 32, 8 * 64 + 32},
    {14 * 64 + 32, 8 * 64 + 32},
    {14 * 64 + 32, 5 * 64 + 32},
    {11 * 64 + 32, 5 * 64 + 32},
    {11 * 64 + 32, 2 * 64 + 32},
    {16 * 64 + 32, 2 * 64 + 32},
    {16 * 64 + 32, 5 * 64 + 32},
    {19 * 64 + 32, 5 * 64 + 32}};
}


void enemy::slow_down()
{
    slowed = true;
}

t_point enemy::get_pos() const
{
    return posPoint;
}

void enemy::display(const game& game)
{
    if (alive == false)
        return;

    SDL_Renderer* renderer = game.displayM.renderer;

    // display the red rectangle (our enemy)
    SDL_SetRenderDrawColor(renderer, 74, 146, 25, 255);
    circleEnemyDisplay.centerOfCircle = {posPoint.x, posPoint.y};
    draw_circle(circleEnemyDisplay, renderer, true);

    // DISPLAY HEALTH BAR //
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {(int)posPoint.x - 24, (int)posPoint.y - 30, 48, 8};
	SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    rect = {(int)posPoint.x - 24, (int)posPoint.y - 30, (health * 48) / definedHealth, 8};
	SDL_RenderFillRect(renderer, &rect);
}

bool enemy::reach_point_pattern() const
{
    if (posPoint.x <= pattern[counterPattern + 1].x + 1
    &&  posPoint.x >= pattern[counterPattern + 1].x - 1
    &&  posPoint.y <= pattern[counterPattern + 1].y + 1
    &&  posPoint.y >= pattern[counterPattern + 1].y - 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void enemy::manage_slow(game& game)
{
    if (slowed == true)
    {
        speed     = definedSpeed / 2;
        slowTime += game.deltaTime;
    }
    if (slowTime > 1)
    {
        slowed   = false;
        speed    = definedSpeed;
        slowTime = 0;
    }
}

void enemy::hit_castle(game& game)
{
    if (posPoint.x >= 1152)
    {
        game.healthCastle -= damage;
        die();
    }
}

void enemy::take_heal(const int& healReceived)
{
    if (health + healReceived <= definedHealth)
    {
        health += healReceived;
    }
}

void enemy::update_enemy(game& game)
{
    display(game);
    move(game);
}

bool enemy::get_appeared() const
{
    return appeared;
}