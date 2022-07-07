#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "display.h"
#include "vector.h"
//#include "explosive_tower.h"

#define MAP_LINE 11
#define MAP_COLUMN 20

class tower;
class enemy;
class slow_tower;
class explosive_tower;

class game
{
    private:
        int                                 money;
        int                                 cursorX;
        int                                 cursorY;
        int                                 level;
        float                               definedTimeBetweenRounds;
        float                               counterTimeBetweenRounds;
        bool                                levelInProgress;

        void                                push_back_bought_tower(int& node);
        void                                init_map_list(std::vector<int>& mapList);
        void                                init_level();
        void                                load_enemy_list(const int& nbBasicEnemies, const int& nbBigEnemies, const int& nbHealers);
        bool                                are_there_enemies_alive();


        std::vector<int>                    mapList;

    public:
        display                             displayM;
        std::vector<std::unique_ptr<enemy>> enemyList;
        std::vector<std::unique_ptr<tower>> towerList;

        float                               deltaTime;
        float                               counterTimeCurrentLevel;

        int                                 chooseTower;
        int                                 healthCastle;
        int                                 nbEnemyOnMap;

        bool                                running;

        void                                buy_tower();
        void                                earn_money(int money);
        void                                lose_money(int money);
        void                                select_tower();
        void                                click_to_upgrade_tower();
        void                                enemy_spawner();
        void                                update_game(Uint32& time);


        int                                 get_money();
        int&                                get_node_map(const int& i, const int& j);
        int                                 get_level();

        game();
        ~game();

};

bool operator==(const t_point& firstDot, const t_point& secondDot);

#endif