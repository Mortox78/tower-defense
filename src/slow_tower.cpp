#include <iostream>
#include <assert.h>
#include "../include/game_manager.h"
#include "../include/slow_tower.h"
#include "../include/tower.h"
#include "../include/collisions.h"
#include "../include/enemy.h"


slow_tower::slow_tower()
{
    damage               = 1;
    range.radCircle      = 130;
    shootingSpeed        = 0.1;
    posXHUD              = 19 * 64 + 32;
    posYHUD              = 4  * 64 + 32;
    init_tower();
}

slow_tower::~slow_tower()
{

}


void slow_tower::init_tower()
{
    localRef.centerRef = {-32, -32};
    localRef.vecRefX.x = 1;
    localRef.vecRefX.y = 0;
    localRef.vecRefY.y = 1;
    localRef.vecRefY.x = 0;

    array.clear();

    array.push_back({0, 24});
    array.push_back({24, 0});
    array.push_back({0, -24});
    array.push_back({-24, 0});
    array.push_back({0, 24});
    array.push_back({0, -24});
    array.push_back({24, 0});
    array.push_back({-24, 0});

    array.push_back({0, 0});
}


void slow_tower::upgrade(game& game)
{
    if (level == 15 || game.get_money() < 5)
        return;

    damage += 0.5;
    shootingSpeed -= 0.001;
    ++level;
    game.lose_money(5);
}

void slow_tower::display_tower(game& game)
{
	if (bought == false)
		return;

    SDL_Renderer* renderer = game.displayM.renderer;
    change_tower_color(game);

	for (int i = 0; i < 7; ++i)
	{
		t_point A = localToWorldPoint(localRef, array[i]);
		t_point B = localToWorldPoint(localRef, array[i + 1]);

		SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);
	}

	if (dotCircle(posEnemyLock, range) && shooting == true)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		t_referential towerRef = localRef;
		t_point towPos		   = towerRef.centerRef;
		SDL_RenderDrawLine(renderer, towPos.x, towPos.y, posEnemyLock.x, posEnemyLock.y);
	}
    else
    {
        // posEnemyLock become the center of the tower
        posEnemyLock = localToWorldPoint(localRef, array[4]);
    }
}

void slow_tower::change_tower_color(game& game)
{
    SDL_Renderer* renderer = game.displayM.renderer;

	if (level < 5)
	    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	else if (level >= 5 && level < 10)
		SDL_SetRenderDrawColor(renderer, 0, 135, 255, 255);
	else if (level >= 10 && level < 15)
		SDL_SetRenderDrawColor(renderer, 212, 77, 232, 255);
	else if (level == 15)
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
}

bool slow_tower::interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game)
{
    t_point posEnemy = enemy->get_pos();
    if (dotCircle(posEnemy, range) && timeBetweenShot > shootingSpeed && shooting == false)
    {
        shooting = true;
        enemy->slow_down();
        timeBetweenShot = 0;
        posEnemyLock = posEnemy;
        enemy->take_damage(damage, game);
        return 1;
    }
    return 0;
}