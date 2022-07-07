#include <iostream>
#include "../include/tower.h"
#include "../include/explosive_tower.h"
#include "../include/collisions.h"
#include "../include/enemy.h"
#include "../include/game_manager.h"



explosive_tower::explosive_tower()
{
    damage                      = 50;
    range.radCircle             = 130;
    rocketLaunched              = false;
    rangeRocket.centerOfCircle  = {0, 0};
    rangeRocket.radCircle       = 48;
    rocketExploded              = false;
    timeRocketExplosion         = 0;
    radDisplayRocketExplo       = 1;
    shootingSpeed               = 2;
    posXHUD                     = 19 * 64 + 32;
    posYHUD                     = 5  * 64 + 20;
    init_rocket();
    init_tower();
}

explosive_tower::~explosive_tower()
{

}


void explosive_tower::init_tower()
{
    localRef.centerRef = {-32, -32};
    localRef.vecRefX.x = 1;
    localRef.vecRefX.y = 0;
    localRef.vecRefY.y = 1;
    localRef.vecRefY.x = 0;

    array.clear();

    // first block
    array.push_back({-20, 7});
    array.push_back({20, 7});
    array.push_back({25, -7});
    array.push_back({15, -2});
    array.push_back({-15, -2});
    array.push_back({-25, -7});
    array.push_back({-20, 7});

    // second block
    array.push_back({-17, 7});
    array.push_back({-17, 10});
    array.push_back({17, 10});
    array.push_back({17, 7});
    array.push_back({17, 10});

    // the cannons
    array.push_back({-15, 10});
    array.push_back({-15, 30});
    array.push_back({-10, 30});
    array.push_back({-10, 10});
    array.push_back({-3, 10});
    array.push_back({-3, 30});
    array.push_back({0, 30});
    array.push_back({3, 30});
    array.push_back({3, 10});
    array.push_back({10, 10});
    array.push_back({10, 30});
    array.push_back({15, 30});
    array.push_back({15, 10});
}


void explosive_tower::shoot_rocket(t_point aimPos, game& game)
{
    this->aimPos = aimPos;

    rocketRef.centerRef = localToWorldPoint(localRef, array[16]);

    vecRocketAimPos = getVectorFromDots(rocketRef.centerRef, aimPos);
    vecRocketAimPos = unitaryVector(vecRocketAimPos);

    rocketRef.vecRefY = vecRocketAimPos;
    rocketRef.vecRefX = getNormalVector(rocketRef.vecRefY);
}

void explosive_tower::move_rocket(game& game)
{
    if (rocketLaunched == false)
        return;

    rocketRef.centerRef.x += vecRocketAimPos.x * 200.f * game.deltaTime;
    rocketRef.centerRef.y += vecRocketAimPos.y * 200.f * game.deltaTime;

    rangeRocket.centerOfCircle = rocketRef.centerRef;

    t_point rocketPos = rocketRef.centerRef;

    if (rocketPos.x <= aimPos.x + 1
    &&  rocketPos.x >= aimPos.x - 1
    &&  rocketPos.y <= aimPos.y + 1
    &&  rocketPos.y >= aimPos.y - 1)
    {
        rocket_exploded(game);
        rocketLaunched = false;
        rocketRef.centerRef.x = -32;
        rocketRef.centerRef.y = -32;
    }
}

void explosive_tower::init_rocket()
{
    // the rocket
    rocketPatternDesign.push_back({-4, 8});
    rocketPatternDesign.push_back({0, 15});
    rocketPatternDesign.push_back({4, 8});
    rocketPatternDesign.push_back({4, 0});
    rocketPatternDesign.push_back({-4, 0});

    // flames at the end of the rocket
    rocketPatternDesign.push_back({-2, 0});
    rocketPatternDesign.push_back({0, -5});
    rocketPatternDesign.push_back({2, 0});

    rocketPatternDesign.push_back({-2, 0});
    rocketPatternDesign.push_back({0, -4});
    rocketPatternDesign.push_back({2, 0});

    rocketPatternDesign.push_back({-1, 0});
    rocketPatternDesign.push_back({0, -3});
    rocketPatternDesign.push_back({1, 0});
}

void explosive_tower::rocket_exploded(game& game)
{
    if (rocketLaunched == false)
        return;

    for (std::unique_ptr<enemy>& enemy : game.enemyList)
    {
        if (dotCircle(enemy->get_pos(), rangeRocket))
        {
            enemy->take_damage(50, game);
        }
    }
    rocketExploded = true;
}

void explosive_tower::reset_time_rocket_explosion()
{
    timeRocketExplosion = 0;
    rocketExploded = false;
    radDisplayRocketExplo = 1;
}

void explosive_tower::add_radius_display_rocket_explosion()
{
    if (radDisplayRocketExplo == 48)
    {
        reset_time_rocket_explosion();
        return;
    }
    radDisplayRocketExplo += 1;
}

void explosive_tower::upgrade(game& game)
{
    if (level == 15 || game.get_money() < 5)
        return;

    damage += 10;
    shootingSpeed -= 0.05;
    ++level;
    game.lose_money(5);
}

void explosive_tower::display_tower(game& game)
{
	if (bought == false)
		return;

    SDL_Renderer* renderer = game.displayM.renderer;
	SDL_SetRenderDrawColor(renderer, 169, 50, 38, 255);

	for (int i = 0; i < 9; ++i)
	{
		t_point A = localToWorldPoint(localRef, array[i]);
		t_point B = localToWorldPoint(localRef, array[i + 1]);

		SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);
	}

    change_tower_color(game);

	for (int i = 10; i < 24; ++i)
	{
		t_point A = localToWorldPoint(localRef, array[i]);
		t_point B = localToWorldPoint(localRef, array[i + 1]);

		SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);	
	}

    this->display_rocket(game);
    this->display_rocket_exploded(game);
    this->move_rocket(game);
}

void explosive_tower::display_rocket(game& game)
{
	if (rocketLaunched == false)
		return;

    SDL_Renderer* renderer = game.displayM.renderer;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 12; ++i)
	{
		t_point A = localToWorldPoint(rocketRef, rocketPatternDesign[i]);
		t_point B = localToWorldPoint(rocketRef, rocketPatternDesign[i + 1]);

		if (i == 5)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
		else if (i == 4)
		{
			A = localToWorldPoint(rocketRef, rocketPatternDesign[0]);
			B = localToWorldPoint(rocketRef, rocketPatternDesign[4]);
		}

		SDL_RenderDrawLine(renderer, A.x, A.y, B.x, B.y);
	}
}

void explosive_tower::display_rocket_exploded(game& game)
{
	if (rocketExploded)
	{
        SDL_Renderer* renderer = game.displayM.renderer;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        rangeDisplayRocket.centerOfCircle = rangeRocket.centerOfCircle;
		rangeDisplayRocket.radCircle      = radDisplayRocketExplo;

		draw_circle(rangeDisplayRocket, renderer, true);

        timeRocketExplosion += game.deltaTime;
		add_radius_display_rocket_explosion();

		if (timeRocketExplosion > 0.5)
		{
			reset_time_rocket_explosion();
		}
	}
}

void explosive_tower::change_tower_color(game& game)
{
    SDL_Renderer* renderer = game.displayM.renderer;

	if (level < 5)
		SDL_SetRenderDrawColor(renderer, 44, 62, 80, 255);
	else if (level >= 5 && level < 10)
		SDL_SetRenderDrawColor(renderer, 0, 135, 255, 255);
	else if (level >= 10 && level < 15)
		SDL_SetRenderDrawColor(renderer, 212, 77, 232, 255);
	else if (level == 15)
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
}

bool explosive_tower::interact_with_enemy(std::unique_ptr<enemy>& enemy, game& game)
{
    t_point posEnemy = enemy->get_pos();
    if (dotCircle(posEnemy, range) && timeBetweenShot > shootingSpeed && shooting == false && rocketLaunched == false)
    {
        shooting = true;
        rocketLaunched = true;
        timeBetweenShot = 0;
        aim_at_enemy(*enemy);
        shoot_rocket(posEnemy, game);
        return 1;
    }
    return 0;
}