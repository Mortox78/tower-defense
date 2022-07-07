#include <iostream>
#include "../include/display.h"
#include "../include/enemy.h"
#include "../include/big_enemy.h"
#include "../include/healer.h"
#include "../include/input.h"
#include "../include/game_manager.h"
#include "../include/slow_tower.h"
#include "../include/explosive_tower.h"

using namespace std;

int main()
{
    game game;

	Uint32 time = 0;

/*
    for (int i = 0; i < MAP_LINE; ++i)
    {
        std::cout << std::endl;
        for (int j = 0; j < MAP_COLUMN; ++j)
        {
            std::cout << "||" << get_node_map(game.mapList, j, i) << "||";
        }
    }
*/

    while (game.running == true)
    {
        game.update_game(time);
    }
/*    for (int i = 0; i < MAP_LINE; ++i)
    {
        std::cout << std::endl;
        for (int j = 0; j < MAP_COLUMN; ++j)
        {
            std::cout << "||" << get_node_map(game.mapList, j, i) << "||";
        }
    }
*/
    return 0;
}