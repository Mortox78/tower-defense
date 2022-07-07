#ifndef _BIG_ENEMY_H_
#define _BIG_ENEMY_H_

#include <iostream>
#include <vector>
#include "vector.h"
#include "game_manager.h"

class enemy;

class big_enemy : public enemy
{
    public:
        big_enemy();
        ~big_enemy();
    
        void            display(const game& game) override;
};



#endif