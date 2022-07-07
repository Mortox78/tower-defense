#include <iostream>
#include "../include/game_manager.h"
#include "../include/slow_tower.h"
#include "../include/vector.h"
#include "../include/tower.h"
#include "../include/enemy.h"
#include "../include/explosive_tower.h"
#include "../include/big_enemy.h"
#include "../include/healer.h"
#include "../include/input.h"

game::game()
{
    running                  = true;
    money                    = 5000;
    chooseTower              = 0;
    healthCastle             = 5;
    nbEnemyOnMap             = 0;
    definedTimeBetweenRounds = 5;
    counterTimeBetweenRounds = 0;
    levelInProgress          = false;
    level                    = 0;

    init_map_list(mapList);
}

game::~game()
{
    SDL_DestroyWindow(displayM.window);
    SDL_DestroyRenderer(displayM.renderer);
}

void game::init_map_list(std::vector<int>& mapList)
{
    mapList = {
    -3,  -3,  -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
    -3,  -3,  -3,  0,  0,  0,  0,  0,  0, -3, -3,  0,  0,  0,  0,  0,  0, -3, -3, -3,
    -3,  -3,   0, -1, -1, -1, -1, -1, -1,  0,  0, -1, -1, -1, -1, -1, -1,  0, -3, -3,
    -3,  -3,   0, -1,  0,  0,  0,  0, -1,  0,  0, -1,  0,  0,  0,  0, -1,  0, -2, 10,
     0,   0,   0, -1,  0,  0,  0,  0, -1,  0,  0, -1,  0,  0,  0,  0, -1,  0, -2, 11,
    -1,  -1,  -1, -1,  0, -1, -1, -1, -1,  0,  0, -1, -1, -1, -1,  0, -1, -1, -2, 12,
     0,   0,   0,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0,  0, -2, 13,
    -3,  -3,  -3, -3,  0, -1,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0, -3, -3, -2, 14,
    -3,  -3,  -3, -3,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -3, -3, -3, -3,
    -3,  -3,  -3, -3, -3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -3, -3, -3, -3, -3,
    -3,  -3,  -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3 };
}

void game::buy_tower()
{
    if (money < 5)
        return;

    SDL_GetMouseState(&cursorX, &cursorY);
    cursorX /= 64;
    cursorY /= 64;

    int& node = get_node_map(cursorX, cursorY);
    if (node != 0)
        return;

    push_back_bought_tower(node);

    cursorX = (cursorX * 64) + 32;
    cursorY = (cursorY * 64) + 32;

    tower& tower = *towerList.back();
    tower.change_pos(cursorX, cursorY);
    tower.set_bought_true();
}

void game::earn_money(int money)
{
    this->money += money;
}

void game::select_tower()
{
    int x;
    int y;

    SDL_GetMouseState(&x, &y);

    x /= 64;
    y /= 64;

    int& node = get_node_map(x, y);

    if (node < 10)
        return;

    if (node == 10 && money >= 5)
    {
        chooseTower = 1;
    }
    else if (node == 11 && money >= 10)
    {
        chooseTower = 2;
    }
    else if (node == 12 && money >= 20)
    {
        chooseTower = 3;
    }
}


int game::get_money()
{
    return money;
}

void game::click_to_upgrade_tower()
{
    int x = 0;
    int y = 0;

    SDL_GetMouseState(&x, &y);
    x /= 64;
    y /= 64;
    int& node = get_node_map(x, y);

    if (node < 1 || node > 4)
        return;

    float X = x * 64 + 32;
    float Y = y * 64 + 32;

    t_point posClickOnScreen = {X, Y};

    for (std::unique_ptr<tower>& tower : towerList)
    {
        if (tower->get_pos() == posClickOnScreen)
        {
            tower->upgrade(*this);
        }
    }
}


bool operator==(const t_point& firstDot, const t_point& secondDot)
{
    if (firstDot.x == secondDot.x && firstDot.y == secondDot.y)
        return true;
    else
        return false;
}

void game::lose_money(int money)
{
    this->money -= money;
}

int& game::get_node_map(const int& i, const int& j)
{
	return mapList[j * MAP_COLUMN + i];
}

void game::push_back_bought_tower(int& node)
{
    if (chooseTower == 1 && money >= 5)
    {
        towerList.push_back(std::make_unique<tower>());
        node = 1;
        money -= 5;
    }
    else if (chooseTower == 2 && money >= 10)
    {
        towerList.push_back(std::make_unique<slow_tower>());
        node = 2;
        money -= 10;
    }
    else if (chooseTower == 3 && money >= 20)
    {
        towerList.push_back(std::make_unique<explosive_tower>());
        node = 3;
        money -= 20;
    }
}

void game::update_game(Uint32& time)
{
    // get the delta time
    Uint32 current_time = SDL_GetTicks();
	deltaTime = (time > 0) ? ((current_time - time) / 1000.f) : 1.0f / 60.0f;
	time = current_time;

    counterTimeCurrentLevel += deltaTime;

    displayM.draw_map(*this);
    player_keyboard(*this);

    // update towers
    for (std::unique_ptr<tower>& tower : towerList)
    {
        tower->display_tower(*this);
        tower->shoot_enemy(*this);
    }
    // update enemies
    for (std::unique_ptr<enemy>& enemy : enemyList)
    {
        enemy->update_enemy(*this);
    }
    if (healthCastle <= 0)
    {
        displayM.display_score(*this);
        running = false;
        return;
    }
    if (are_there_enemies_alive() == false)
    {
        init_level();
    }
    enemy_spawner();
    displayM.display_HUD(*this);
    displayM.display_renderer(*this);
}

void game::init_level()
{
    // when levelInProgress == false, it means that it is the break before the next round
    if (counterTimeBetweenRounds < definedTimeBetweenRounds)
    {
        counterTimeBetweenRounds += deltaTime;
        return;
    }
    else if (counterTimeBetweenRounds > definedTimeBetweenRounds)
    {
        levelInProgress          = true;
        counterTimeBetweenRounds = 0;
    }

    // if it's the end of the break, load a new level with new enemies
    if (levelInProgress == true)
    {
        ++level;
        counterTimeCurrentLevel = 0;
        int nbBasicEnemies = level * 4;
        int nbBigEnemies   = nbBasicEnemies / 2;
        int nbHealers      = nbBigEnemies / 2;
        load_enemy_list(nbBasicEnemies, nbBigEnemies, nbHealers);
    }
}

bool game::are_there_enemies_alive()
{
    // check if there is at least one enemy alive in our list
    for (std::unique_ptr<enemy>& enemy : enemyList)
    {
        if (enemy->get_alive() == true || enemy->get_appeared() == false)
        {
            return true;
        }
    }

    // if every enemy are dead, reset the counter of enemy
    levelInProgress = false;
    nbEnemyOnMap = 0;
    enemyList.clear();
    return false;    
}

void game::load_enemy_list(const int& nbBasicEnemies, const int& nbBigEnemies, const int& nbHealers)
{
    for (int i = 0; i < nbBasicEnemies; ++i)
    {
        enemyList.push_back(std::make_unique<enemy>());
    }
    for (int i = 0; i < nbBigEnemies; ++i)
    {
        enemyList.push_back(std::make_unique<big_enemy>());
    }
    for (int i = 0; i < nbHealers; ++i)
    {
        enemyList.push_back(std::make_unique<healer>());
    }


    int indexNbEnemyInList = 0;
    for (std::unique_ptr<enemy>& enemy : enemyList)
    {
        enemy->identifier = indexNbEnemyInList;
        ++indexNbEnemyInList;
    }
}

int game::get_level()
{
    return level;
}

void game::enemy_spawner()
{
    int i = counterTimeCurrentLevel / 1;

    for (std::unique_ptr<enemy>& enemy : enemyList)
    {
        if (enemy->get_appeared() == false && i == nbEnemyOnMap)
        {
            enemy->spawn();
            ++nbEnemyOnMap;
        }
    }
}