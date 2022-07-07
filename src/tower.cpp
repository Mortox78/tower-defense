#include <iostream>
#include <assert.h>
#include "../include/tower.h"
#include "../include/collisions.h"
#include "../include/vector.h"
#include "../include/enemy.h"
#include "../include/game_manager.h"
#include <SDL2/SDL.h>

tower::tower()
{
    damage               = 10;
    range.radCircle      = 100;
    timeBetweenShot      = 0;
    range.centerOfCircle = localRef.centerRef;
    shooting             = false;
    bought               = false;
    shootingSpeed        = 0.3;
    posXHUD              = 19 * 64 + 32;
    posYHUD              = 3  * 64 + 16;
    level                = 0;
    init_tower();
}

tower::~tower()
{

}


void tower::shoot_enemy(game& game)
{
    if (timeBetweenShot > shootingSpeed)
    {
        shooting = false;
    }
    else
    {
        timeBetweenShot += game.deltaTime;
        return;
    }
    
    for (std::unique_ptr<enemy>& enemy : game.enemyList)
    {
        if (enemy->get_alive() == true)
        {
            if (interact_with_enemy(enemy, game))
                return;
        }
    }
}

void tower::init_tower()
{
    localRef.centerRef = {-32, -32};
    localRef.vecRefX.x = 1;
    localRef.vecRefX.y = 0;
    localRef.vecRefY.y = 1;
    localRef.vecRefY.x = 0;

    // the tower
    array.push_back({-20, 10});
    array.push_back({20, 10});
    array.push_back({25, -10});
    array.push_back({-25, -10});
    array.push_back({-20, 10});

    array.push_back({-10, 10});
    array.push_back({-10, 40});
    array.push_back({-5, 40});
    array.push_back({-5, 10});

    array.push_back({10, 10});
    array.push_back({10, 40});
    array.push_back({5, 40});
    array.push_back({5, 10});

    array.push_back({-10, 10});

    // the flames at the end of the cannons
    array.push_back({-10, 40});
    array.push_back({-7, 50});
    array.push_back({-5, 40});

    array.push_back({-9, 41});
    array.push_back({-7, 49});
    array.push_back({-6, 41});

    array.push_back({-8, 42});
    array.push_back({-7, 48});
    array.push_back({-7, 42});

    array.push_back({10, 40});
    array.push_back({7, 50});
    array.push_back({5, 40});

    array.push_back({9, 41});
    array.push_back({7, 49});
    array.push_back({6, 41});

    array.push_back({8, 42});
    array.push_back({7, 48});
    array.push_back({7, 42});
}


void tower::aim_at_enemy(enemy& enemy)
{
    if (enemy.get_alive() == false)
        return;

    t_point posEnemy = {enemy.get_pos().x, enemy.get_pos().y};
    t_vector2 vecTowerToEnemy = getVectorFromDots(localRef.centerRef, posEnemy);

    localRef.vecRefY = unitaryVector(vecTowerToEnemy);
    localRef.vecRefX = rotateVectorWith90Angle(localRef.vecRefY);
}

void tower::change_pos(const int& x, const int& y)
{
    localRef.centerRef.x = x;
    localRef.centerRef.y = y;

    range.centerOfCircle.x = x;
    range.centerOfCircle.y = y;
}


void tower::set_bought_true()
{
    bought = true;
}


t_point tower::get_pos() const
{
    return localRef.centerRef;
}

void tower::upgrade(game& game)
{
    if (level == 15 || game.get_money() < 5)
        return;

    damage          += 1;
    shootingSpeed   -= 0.003;
    ++level;

    game.lose_money(5);
}


void tower::display_tower(game& game)
{
	if (bought == false)
		return;

    SDL_Renderer* renderer = game.displayM.renderer;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // draw the tower
    for (int i = 0; i < 14; ++i)
    {
        if (i == 6)
        {
            change_tower_color(game);
        }

        t_point A = localToWorldPoint(localRef, array[i]);
		t_point B = localToWorldPoint(localRef, array[i + 1]);

        SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);
    }

    // draw the flames
	if (shooting && (timeBetweenShot / 0.015 < 10))
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (int i = 14; i < 30; ++i)
		{
			t_point A = localToWorldPoint(localRef, array[i]);
			t_point B = localToWorldPoint(localRef, array[i + 1]);

			SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);
			if (i == 20)
			{
				i += 2;
			}
		}
	}
}

void tower::display_tower_HUD(game& game)
{
    localRef.centerRef.x = posXHUD;
    localRef.centerRef.y = posYHUD;
    bought               = true;

    display_tower(game);
}

void tower::display_buying_tower(game& game)
{
    int x = 0;
    int y = 0;

	SDL_GetMouseState(&x, &y);
    change_pos(x, y);
    
    SDL_Renderer* renderer = game.displayM.renderer;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	draw_circle(range, renderer, false);
    bought = true;
    display_tower(game);
}

void tower::change_tower_color(game& game)
{
    SDL_Renderer* renderer = game.displayM.renderer;
    
	if (level < 5)
		SDL_SetRenderDrawColor(renderer, 39, 55, 70, 255);
	else if (level >= 5 && level < 10)
		SDL_SetRenderDrawColor(renderer, 0, 135, 255, 255);
	else if (level >= 10 && level < 15)
		SDL_SetRenderDrawColor(renderer, 212, 77, 232, 255);
	else if (level == 15)
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
}

bool tower::interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game)
{
    t_point posEnemy = enemy->get_pos();
    if (dotCircle(posEnemy, range) && timeBetweenShot > shootingSpeed)
    {
        shooting = true;
        enemy->take_damage(damage, game);
        timeBetweenShot = 0;
        aim_at_enemy(*enemy);
        return 1;
    }
    return 0;
}